// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseWidget.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"

void UPauseWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	
}

void UPauseWidget::OnSettingsPressed()
{
	UE_LOG(LogTemp, Warning, TEXT("Settingsss") );
	if (MenuSwitcher)
	{
		MenuSwitcher->SetActiveWidget(SettingsMenuPanel);
	}
}

void UPauseWidget::OnBackPressed()
{
	if (MenuSwitcher)
	{
		MenuSwitcher->SetActiveWidget(PauseMenuPanel);
	}
}
