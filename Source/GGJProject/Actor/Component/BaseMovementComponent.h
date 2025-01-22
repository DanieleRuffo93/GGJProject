#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseMovementComponent.generated.h"

class USplineComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GGJPROJECT_API UBaseMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UBaseMovementComponent();

protected:
	virtual void BeginPlay() override;

public:
	
	UPROPERTY(EditAnywhere, Category = "Patrol")
	float PatrolDistance = 200.0f;

	UPROPERTY(EditAnywhere, Category = "Patrol")
	float MovementSpeed = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Patrol")
	USplineComponent* SplineToFollow;
	
	UPROPERTY(EditAnywhere, Category = "Patrol|Ground Check")
	float GroundCheckDistance = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Patrol|Ground Check")
	float EdgeCheckOffset = 50.0f; 
	
	FVector StartLocation;
	FVector CurrentDirection;
	bool bInitialized = false;
	float DistanceTraveled = 0.0f;
	float CurrentDistance = 0.0f;
	bool bMovingForward = true;
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	bool IsGroundAhead() const;
	FVector GetNextDirectionFromSpline() const;
};
