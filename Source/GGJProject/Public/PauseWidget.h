// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseWidget.generated.h"

class UButton;
class UWidgetSwitcher;
/**
 * 
 */
UCLASS()
class GGJPROJECT_API UPauseWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> MenuSwitcher;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWidget> PauseMenuPanel;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWidget> SettingsMenuPanel;

	UFUNCTION(BlueprintCallable)
	void OnSettingsPressed();

	UFUNCTION(BlueprintCallable)
	void OnBackPressed();
	

	
};
