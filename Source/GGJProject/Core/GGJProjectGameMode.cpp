// Copyright Epic Games, Inc. All Rights Reserved.

#include "GGJProjectGameMode.h"
#include "../Actor/GGJProjectCharacter.h"
#include "GGJProject/Actor/Component/SuperellipseOrbitComponent.h"
#include "UObject/ConstructorHelpers.h"

AGGJProjectGameMode::AGGJProjectGameMode()
{
	OrbitComponent = CreateDefaultSubobject<USuperellipseOrbitComponent>(TEXT("OrbitComponent"));

}

void AGGJProjectGameMode::BeginPlay()
{
	Super::BeginPlay();
}
