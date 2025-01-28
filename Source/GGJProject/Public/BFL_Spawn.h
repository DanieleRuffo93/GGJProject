// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BFL_Spawn.generated.h"

/**
 * 
 */
UCLASS()
class GGJPROJECT_API UBFL_Spawn : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, Category="Level Spawning")
	static void SpawnLevelAtLocation(UObject* WorldContextObject, const FString& LevelName, const FVector& Location, const FRotator& Rotation);
};
