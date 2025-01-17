// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuBase.generated.h"

/**
 * 
 */
UCLASS()
class GGJPROJECT_API UMenuBase : public UUserWidget
{
	GENERATED_BODY()

public:
    virtual bool Initialize() override;

protected:
    UPROPERTY(meta = (BindWidget))
    class UButton* PlayButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* OptionsButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* ExitButton;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menu", meta = (AllowedClasses = "World"))
    TSoftObjectPtr<UWorld> LevelToLoad;

    UFUNCTION()
    void OnPlayClicked();

    UFUNCTION()
    void OnOptionsClicked();

    UFUNCTION()
    void OnExitClicked();
	
};
