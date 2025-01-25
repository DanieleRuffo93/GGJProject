// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SplinePathActor.generated.h"

class USplineComponent;

UCLASS()
class GGJPROJECT_API ASplinePathActor : public AActor
{
	GENERATED_BODY()

	
public:	
	// Sets default values for this actor's properties
	ASplinePathActor();

	/** Spline path detemines a path to follow */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USplineComponent* Spline;

protected:


public:	


};
