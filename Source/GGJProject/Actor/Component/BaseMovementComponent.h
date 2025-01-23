#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseMovementComponent.generated.h"

class USplineComponent;
class USuperellipseOrbitComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GGJPROJECT_API UBaseMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UBaseMovementComponent();

protected:
	virtual void BeginPlay() override;

public:
	
	UPROPERTY(EditAnywhere, Category = "Movemement")
	float RotationSpeed { 20.f };

	UPROPERTY(EditAnywhere, Category = "Movement")
	float MovementDirection { 1.0f };
	
	UPROPERTY(EditAnywhere, Category = "Movement|Ground Check")
	float GroundCheckDistance { 100.0f };

	UPROPERTY(EditAnywhere, Category = "Movement|Ground Check")
	float EdgeCheckOffset { 50.0f };

	
	UPROPERTY()
	bool bIsOrbitInitialized { false };
	bool bDelegateRegistered { false };
	float CurrentAngle { 0.0f };
	UPROPERTY()
	TObjectPtr<USuperellipseOrbitComponent> OrbitComponent;
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	bool IsGroundAhead() const;
	UFUNCTION()
	void OnOrbitReady();
	
};
