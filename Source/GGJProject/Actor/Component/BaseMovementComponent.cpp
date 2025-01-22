// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseMovementComponent.h"

#include "Components/SplineComponent.h"

UBaseMovementComponent::UBaseMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	CurrentDirection = FVector(1.0f, 0.0f, 0.0f);
}

void UBaseMovementComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UBaseMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	AActor* Owner = GetOwner();
	if (!Owner || !SplineToFollow) return;
	
	if (!IsGroundAhead())
	{
		bMovingForward = !bMovingForward;
		return;
	}
	
	float MovementThisFrame = MovementSpeed * DeltaTime;
	if (!bMovingForward)
	{
		MovementThisFrame = -MovementThisFrame;
	}
	
	CurrentDistance += MovementThisFrame;

	FVector NewLocation = SplineToFollow->GetLocationAtDistanceAlongSpline(
		CurrentDistance, 
		ESplineCoordinateSpace::World
	);
	Owner->SetActorLocation(NewLocation);
	FRotator NewRotation = SplineToFollow->GetRotationAtDistanceAlongSpline(
		CurrentDistance, 
		ESplineCoordinateSpace::World
	);
	Owner->SetActorRotation(NewRotation);
}

bool UBaseMovementComponent::IsGroundAhead() const
{
	AActor* Owner = GetOwner();
	if (!Owner || !SplineToFollow) return false;
	
	FVector SplineDirection = GetNextDirectionFromSpline();
	FVector CurrentLocation = Owner->GetActorLocation();
	FVector TraceStart = CurrentLocation + (SplineDirection * EdgeCheckOffset);
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
		0.1f,
		0,
		1.0f
	);
#endif

	return bHit;
}

FVector UBaseMovementComponent::GetNextDirectionFromSpline() const
{
	if (!SplineToFollow) return FVector::ForwardVector;
	
	float NextDistance = CurrentDistance + (bMovingForward ? EdgeCheckOffset : -EdgeCheckOffset);
	return SplineToFollow->GetDirectionAtDistanceAlongSpline(
		NextDistance,
		ESplineCoordinateSpace::World
	).GetSafeNormal();
}
