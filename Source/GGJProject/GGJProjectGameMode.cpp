// Copyright Epic Games, Inc. All Rights Reserved.

#include "GGJProjectGameMode.h"
#include "GGJProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"

AGGJProjectGameMode::AGGJProjectGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
