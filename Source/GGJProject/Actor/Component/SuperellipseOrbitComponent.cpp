#include "SuperellipseOrbitComponent.h"

#include "Kismet/GameplayStatics.h"

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
			//CenterLocation = Origin + (Extent * 0.5f);
			CenterActorExtent = FVector2D(Extent.X, Extent.Y);
			UE_LOG(LogTemp, Warning, TEXT("Origin: %s"), *Origin.ToString());
			UE_LOG(LogTemp, Warning, TEXT("Extent: %s"), *Extent.ToString());
			UE_LOG(LogTemp, Warning, TEXT("CenterLocation: %s"), *CenterLocation.ToString());
			

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
	UE_LOG(LogTemp, Warning, TEXT("Lunghezza orbita: %f"), OrbitLength);
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
	UE_LOG(LogTemp, Warning, TEXT("RotationSpeed: %f, MovementDirection: %d, CurrentAngle: %f"), RotationSpeed, MovementDirection, CurrentAngle);
	float DeltaAngle = FMath::DegreesToRadians(RotationSpeed) * DeltaTime * MovementDirection;
	UE_LOG(LogTemp, Warning, TEXT("DeltaAngle: %f"), DeltaAngle);
	CurrentAngle += DeltaAngle;
	
	CurrentAngle = FMath::Fmod(CurrentAngle, 2.0f * PI);
	if (CurrentAngle < 0.0f)
	{
		CurrentAngle += 2.0f * PI;
	}
	UE_LOG(LogTemp, Warning, TEXT("Final CurrentAngle: %f"), CurrentAngle);
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
