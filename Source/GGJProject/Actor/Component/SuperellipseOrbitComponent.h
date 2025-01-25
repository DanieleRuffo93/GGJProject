// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SuperellipseOrbitComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOrbitInitializedSignature);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GGJPROJECT_API USuperellipseOrbitComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USuperellipseOrbitComponent();
	
	UPROPERTY(VisibleAnywhere, Category = "Orbit")
	TObjectPtr<AActor> CenterActor;

	UPROPERTY(EditAnywhere, Category = "Orbit")
	float OrbitRadius { 200.0f };
	
	UPROPERTY(EditAnywhere, Category = "Orbit")
	float SuperellipseExponent { 4.0f };

	UPROPERTY(BlueprintAssignable)
	FOrbitInitializedSignature OnOrbitInitialized;
	
	UPROPERTY(EditAnywhere, Category = "Orbit|Debug")
	bool bDrawDebugSuperellipse {false};

	UPROPERTY(EditAnywhere, Category = "Orbit|Debug")
	float DrawDebugZOffset {0.0f};
	
	
	FVector CenterLocation { FVector::ZeroVector };
	FVector2D CenterActorExtent { FVector::ZeroVector };
	float OrbitLength {0};
	bool bHasBeenInitialized {false};
	
	
	FVector2D CalculatePosition(float Angle, float RadiusOverride = -1.f) const;
	float CalculateDeltaAngle(float CurrentAngle, float DeltaTime, float RotationSpeed, int8 MovementDirection) const;
	FVector GetTangentDirection(float Angle, int32 Direction) const;
	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent);
	void FindCenterActor();
	void CalculateOrbitLength();
	void DrawOrbit(AActor* Actor);

	UFUNCTION(BlueprintCallable)
	void GetCenterAndSqrRadius(FVector& Center, float& SqrRadius);

protected:
	virtual void BeginPlay() override;

		
};
