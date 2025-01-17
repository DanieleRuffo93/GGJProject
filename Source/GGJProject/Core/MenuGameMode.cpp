// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuGameMode.h"
#include "Blueprint/UserWidget.h"

void AMenuGameMode::BeginPlay()
{
    if (MenuWidgetClass)
    {
        CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), MenuWidgetClass);
        if (CurrentWidget)
        {
            CurrentWidget->AddToViewport();
        }
    }
}
