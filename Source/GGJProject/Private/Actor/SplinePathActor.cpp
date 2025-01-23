// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/SplinePathActor.h"
#include "Components/SplineComponent.h"

// Sets default values
ASplinePathActor::ASplinePathActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline Path"));
	Spline->SetupAttachment(RootComponent);

}

