#include "SuperellipseOrbitComponent.h"

#include "Engine/StaticMeshActor.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

USuperellipseOrbitComponent::USuperellipseOrbitComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USuperellipseOrbitComponent::BeginPlay()
{
	Super::BeginPlay();
	
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle,
		[this]()
		{
			FindCenterActor();
		},
		0.1f, 
		false
	);
}

void USuperellipseOrbitComponent::FindCenterActor()
{
	if (!IsValid(CenterActor))
	{
		TArray<AActor*> ActorsWithTag;
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Center"), ActorsWithTag);
		
		if (ActorsWithTag.Num() > 0)
		{
			CenterActor = ActorsWithTag[0];
			CenterLocation = CenterActor->GetActorLocation();
			FVector Origin, Extent;
			CenterActor->GetActorBounds(true, Origin, Extent);
			// TODO Calculate CenterLocation independently from Origin location
			//CenterLocation = Origin + (Extent * 0.5f);
			CenterActorExtent = FVector2D(Extent.X, Extent.Y);

			CalculateOrbitLength();
			if (!bHasBeenInitialized)
			{
				bHasBeenInitialized = true;
				OnOrbitInitialized.Broadcast();
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Nessun attore trovato con il tag 'Center'."));
		}
	}
}

void USuperellipseOrbitComponent::CalculateOrbitLength()
{
	float DeltaAngle = 0.01f;
	float TotalLength = 0.0f;
    
	for (float Angle = 0.0f; Angle < 2.0f * PI; Angle += DeltaAngle)
	{
		FVector2D Pos1 = CalculatePosition(Angle);
		FVector2D Pos2 = CalculatePosition(Angle + DeltaAngle);

		float DeltaX = Pos2.X - Pos1.X;
		float DeltaY = Pos2.Y - Pos1.Y;
		
		TotalLength = FMath::Sqrt(DeltaX * DeltaX + DeltaY * DeltaY);
	}

	OrbitLength = TotalLength;
}

FVector2D USuperellipseOrbitComponent::CalculatePosition(float Angle, float RadiusOverride) const
{
	float Cos{ FMath::Cos(Angle) };
	float Sin{ FMath::Sin(Angle)};
	float AbsCos{ FMath::Abs(Cos)};
	float AbsSin{ FMath::Abs(Sin)};
    
	float R = RadiusOverride > 0.0f ? RadiusOverride : FMath::Pow(
		FMath::Pow(AbsCos, SuperellipseExponent) + 
		FMath::Pow(AbsSin, SuperellipseExponent),
		-1.0f / SuperellipseExponent
	);
    
	float NewX = CenterLocation.X + (CenterActorExtent.X + OrbitRadius) * R * Cos;
	float NewY = CenterLocation.Y + (CenterActorExtent.Y + OrbitRadius) * R * Sin;

	return FVector2D(NewX, NewY);
}

float USuperellipseOrbitComponent::CalculateDeltaAngle(float CurrentAngle, float DeltaTime, float RotationSpeed, int8 MovementDirection ) const
{
	float DeltaAngle = FMath::DegreesToRadians(RotationSpeed) * DeltaTime * MovementDirection;
	CurrentAngle += DeltaAngle;
	
	CurrentAngle = FMath::Fmod(CurrentAngle, 2.0f * PI);
	if (CurrentAngle < 0.0f)
	{
		CurrentAngle += 2.0f * PI;
	}
	return CurrentAngle;
}

FVector USuperellipseOrbitComponent::GetTangentDirection(float Angle, int32 Direction) const
{
	return FVector(
		-FMath::Sin(Angle) * Direction,
		FMath::Cos(Angle) * Direction,
		0.0f
	).GetSafeNormal();
}
void USuperellipseOrbitComponent::GetCenterAndSqrRadius(FVector& Center, float& SqrRadius)
{
	if (CenterActor.Get())
	{
		Center = CenterActor.Get()->GetActorLocation();
	}

	SqrRadius = CalculatePosition(0).Length();
}


void USuperellipseOrbitComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	AActor* Owner = GetOwner();
	if (!IsValid(Owner) || !Owner->IsA<AStaticMeshActor>())
	{
		return;
	}

	if (PropertyChangedEvent.Property && PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(USuperellipseOrbitComponent, OrbitRadius) ||
		PropertyChangedEvent.Property && PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(USuperellipseOrbitComponent, SuperellipseExponent) ||
		PropertyChangedEvent.Property && PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(USuperellipseOrbitComponent, DrawDebugZOffset)||
		PropertyChangedEvent.Property && PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(USuperellipseOrbitComponent, bDrawDebugSuperellipse))
	{
		if (bDrawDebugSuperellipse)
		{
			DrawOrbit(GetOwner());
		}
	}
	
	if (PropertyChangedEvent.Property && PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(USuperellipseOrbitComponent, bDrawDebugSuperellipse))
	{
		if (bDrawDebugSuperellipse)
		{
			DrawOrbit(GetOwner());
		}
		else
		{
			FlushDebugStrings(GetWorld());
			FlushPersistentDebugLines(GetWorld());
		}
	}
}

void USuperellipseOrbitComponent::DrawOrbit(AActor*  Actor)

{
	if (!IsValid(Actor) || !Actor->IsA<AStaticMeshActor>() || !bDrawDebugSuperellipse) 
	{
		return;
	}

	FlushDebugStrings(GetWorld());
	FlushPersistentDebugLines(GetWorld());	
	float DeltaAngle = 0.01f;
	FVector2D PreviousPoint = CalculatePosition(0.0f);

	CenterLocation = Actor->GetActorLocation();
	FVector Origin, Extent;
	Actor->GetActorBounds(true, Origin, Extent);
	// TODO Calculate CenterLocation independently from Origin location
	//CenterLocation = Origin + (Extent * 0.5f);
	CenterActorExtent = FVector2D(Extent.X, Extent.Y);
	
	for (float Angle = DeltaAngle; Angle < 2.0f * PI; Angle += DeltaAngle)
	{
		FVector2D CurrentPoint = CalculatePosition(Angle);
		
		DrawDebugLine(
			GetWorld(),
			FVector{ PreviousPoint.X, PreviousPoint.Y,Actor->GetActorLocation().Z + DrawDebugZOffset },
			FVector{ CurrentPoint.X, CurrentPoint.Y, Actor->GetActorLocation().Z + DrawDebugZOffset },
			FColor::Green,
			true,
			-1.0f, 
			0, 
			2.0f 
		);
		DrawDebugPoint(
			GetWorld(),
			FVector{ CurrentPoint.X, CurrentPoint.Y, Actor->GetActorLocation().Z + DrawDebugZOffset },
			10.0f,
			FColor::Red,
			true,
			-1.0f,
			0 
		);

		PreviousPoint = CurrentPoint;
	}
}