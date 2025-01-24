// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GGJProjectGameMode.generated.h"

class UGameHUD;

UCLASS(minimalapi)
class AGGJProjectGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	AGGJProjectGameMode();

	UFUNCTION(BlueprintCallable, Category = "Pause")
	void PauseGame();

	UFUNCTION(BlueprintCallable, Category = "Pause")
	void ResumeGame();

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> GameHUDClass;
};



