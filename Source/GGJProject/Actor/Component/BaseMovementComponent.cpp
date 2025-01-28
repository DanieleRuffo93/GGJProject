#include "BaseMovementComponent.h"
#include "SuperellipseOrbitComponent.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"

UBaseMovementComponent::UBaseMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UBaseMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	AGameModeBase* GameMode = UGameplayStatics::GetGameMode(this);
	if (GameMode)
	{
		USuperellipseOrbitComponent* OrbitComp = GameMode->FindComponentByClass<USuperellipseOrbitComponent>();
		if (OrbitComp)
		{
			OrbitComponent = OrbitComp;
			if (OrbitComponent->bHasBeenInitialized)
			{
				OnOrbitReady();
			}
			else
				if (!bDelegateRegistered)
				{
					OrbitComponent->OnOrbitInitialized.AddDynamic(this, &UBaseMovementComponent::OnOrbitReady);
					bDelegateRegistered = true;
				}
		}
	}
}

void UBaseMovementComponent::OnOrbitReady()
{
	AActor* Owner = GetOwner();
	if (!IsValid(Owner)) return;
	
	FVector DirectionToCurrent = Owner->GetActorLocation() - OrbitComponent->CenterLocation;
	DirectionToCurrent.Z = 0.0f;
	CurrentAngle = FMath::Atan2(DirectionToCurrent.Y, DirectionToCurrent.X);
	CurrentAngle = FMath::Fmod(CurrentAngle + 2.0f * PI, 2.0f * PI);
	bIsOrbitInitialized = true;
}


void UBaseMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(!bIsOrbitInitialized) return;
	
	AActor* Owner = GetOwner();
	if (!IsValid(Owner) || !IsValid(OrbitComponent)) return;
	
	if (!IsGroundAhead())
	{
		MovementDirection = -MovementDirection;
	}

	//CurrentAngle = OrbitComponent->CalculateDeltaAngle(CurrentAngle, DeltaTime, RotationSpeed, MovementDirection );
	
	float DeltaAngle = FMath::DegreesToRadians(RotationSpeed) * DeltaTime * MovementDirection;
	CurrentAngle += DeltaAngle;
	
	CurrentAngle = FMath::Fmod(CurrentAngle, 2.0f * PI);
	if (CurrentAngle < 0.0f)
	{
		CurrentAngle += 2.0f * PI;
	}
	
	FVector2D NewLocation = OrbitComponent->CalculatePosition(CurrentAngle);
	FVector TangentDirection = OrbitComponent->GetTangentDirection(CurrentAngle, MovementDirection);
	
	Owner->SetActorLocation(FVector(NewLocation.X, NewLocation.Y, GetOwner()->GetActorLocation().Z));
	Owner->SetActorRotation(TangentDirection.Rotation());
}

bool UBaseMovementComponent::IsGroundAhead() const
{
	AActor* Owner = GetOwner();
	if (!Owner) return false;
	
	float TangentAngle = CurrentAngle + (PI / 2.0f) * MovementDirection;
	FVector TangentDirection(FMath::Cos(TangentAngle), FMath::Sin(TangentAngle), 0.0f);
	FVector CurrentLocation = Owner->GetActorLocation();
	FVector TraceStart = CurrentLocation + (TangentDirection * EdgeCheckOffset);
	FVector TraceEnd = TraceStart - FVector(0.0f, 0.0f, GroundCheckDistance);

	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(Owner);

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		TraceStart,
		TraceEnd,
		ECC_Visibility,
		QueryParams
	);

#if WITH_EDITOR
	DrawDebugLine(
		GetWorld(),
		TraceStart,
		TraceEnd,
		bHit ? FColor::Green : FColor::Red,
		false,
		0.1f
	);
#endif

	return bHit;
}