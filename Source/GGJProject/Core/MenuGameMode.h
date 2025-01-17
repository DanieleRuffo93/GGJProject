// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MenuGameMode.generated.h"

class UUSerWidget;

UCLASS()
class GGJPROJECT_API AMenuGameMode : public AGameMode
{
	GENERATED_BODY()

public:
    virtual void BeginPlay() override;

protected:
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UUserWidget> MenuWidgetClass;

    UPROPERTY()
    TObjectPtr<UUserWidget> CurrentWidget;
	
};
