// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PauseWidget.h"
#include "MenuBase.generated.h"

/**
 * 
 */
UCLASS()
class GGJPROJECT_API UMenuBase : public UPauseWidget
{
	GENERATED_BODY()

public:
    virtual bool Initialize() override;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menu", meta = (AllowedClasses = "World"))
    TSoftObjectPtr<UWorld> LevelToLoad;

    UFUNCTION(BlueprintCallable)
    void OnPlayClicked();

    UFUNCTION(BlueprintCallable)
    void OnExitClicked();
	
};
