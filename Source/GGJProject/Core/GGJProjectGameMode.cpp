// Copyright Epic Games, Inc. All Rights Reserved.

#include "GGJProjectGameMode.h"

#include "AbilityIcon.h"
#include "GameHUD.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"


void AGGJProjectGameMode::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Here") );
	
	if (GameHUDClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("Here2") );
		UGameHUD* GameHUD{ CreateWidget<UGameHUD>(GetWorld(), GameHUDClass)};
		if (GameHUD)
		{
			UE_LOG(LogTemp, Warning, TEXT("Here3") );
			GameHUD->AddToViewport();
			// GameHUD->InitializeHUD(); // Configurazione iniziale
		}
	}
	
		/*
	UE_LOG(LogTemp, Warning, TEXT("Here") );
	if (GameHUDClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("Here2") );
		UAbilityIcon* GameHUD{ CreateWidget<UAbilityIcon>(GetWorld(), GameHUDClass)};
		if (GameHUD)
		{
			UE_LOG(LogTemp, Warning, TEXT("Here3") );
			GameHUD->AddToViewport();
			// GameHUD->InitializeHUD(); // Configurazione iniziale
		}
	}
	*/
}

AGGJProjectGameMode::AGGJProjectGameMode()
{
	
}

void AGGJProjectGameMode::PauseGame()
{
	UGameplayStatics::SetGamePaused(GetWorld(), true);
	
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		PlayerController->bShowMouseCursor = true;
		
		PlayerController->SetInputMode(FInputModeUIOnly());
	}
}

void AGGJProjectGameMode::ResumeGame()
{
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		PlayerController->bShowMouseCursor = false;
		
		PlayerController->SetInputMode(FInputModeGameOnly());
	}
}
