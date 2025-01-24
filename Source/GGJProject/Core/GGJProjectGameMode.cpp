// Copyright Epic Games, Inc. All Rights Reserved.

#include "GGJProjectGameMode.h"
#include "GGJProject/Actor/Component/SuperellipseOrbitComponent.h"

AGGJProjectGameMode::AGGJProjectGameMode()
{
	OrbitComponent = CreateDefaultSubobject<USuperellipseOrbitComponent>(TEXT("OrbitComponent"));
}

void AGGJProjectGameMode::BeginPlay()
{
	Super::BeginPlay();
}
