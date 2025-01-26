// Copyright Epic Games, Inc. All Rights Reserved.

#include "GGJProjectGameMode.h"

#include "AbilityIcon.h"
#include "GameHUD.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "GGJProject/Actor/Component/SuperellipseOrbitComponent.h"


void AGGJProjectGameMode::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Here") );
	
	if (GameHUDClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("Here2") );
		GameHUD = CreateWidget<UGameHUD>(GetWorld(), GameHUDClass);
		if (GameHUD)
		{
			UE_LOG(LogTemp, Warning, TEXT("Here3") );
			GameHUD->AddToViewport();
		}
	}

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	if (PlayerController)
	{
		PlayerController->bShowMouseCursor = false;
		PlayerController->SetInputMode(FInputModeGameOnly());
	}
	

}

AGGJProjectGameMode::AGGJProjectGameMode()
{
	OrbitComponent = CreateDefaultSubobject<USuperellipseOrbitComponent>(TEXT("OrbitComponent"));
}


void AGGJProjectGameMode::PauseGame()
{
	UGameplayStatics::SetGamePaused(GetWorld(), true);
	
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	PauseWidget = CreateWidget<UUserWidget>(GetWorld(), PauseMenuWidgetClass);
	if (PlayerController && IsValid(PauseWidget))
	{
		PauseWidget->AddToViewport();
		PlayerController->bShowMouseCursor = true;
		PlayerController->SetInputMode(FInputModeUIOnly());
	}
}

void AGGJProjectGameMode::ResumeGame()
{
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	if (PlayerController && IsValid(PauseWidget))
	{
		PauseWidget->RemoveFromParent();
		PlayerController->bShowMouseCursor = false;
		PlayerController->SetInputMode(FInputModeGameOnly());
	}
}

void AGGJProjectGameMode::QuitGame()
{
	UE_LOG(LogTemp, Warning, TEXT("Inside Quit!!") );
	
	if (IsValid(PauseWidget))
	{
		PauseWidget->RemoveFromParent();
	}
	bool bIsLevelValid { LevelToLoad.IsValid() };
	bool bIsLevelPending { LevelToLoad.IsPending() };
	if ( bIsLevelValid || bIsLevelPending )
	{
		FString LevelName = LevelToLoad.GetAssetName();
		UE_LOG(LogTemp, Warning, TEXT("Loading Level: %s"), *LevelName);
		UGameplayStatics::OpenLevel(this, FName(*LevelName));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("LevelToLoad is not set!"));
	}
}

void AGGJProjectGameMode::EnableAbility(const EAbilities Ability)
{
	UGameHUD* GameHUDInstance { Cast<UGameHUD>(GameHUD)};
	if (IsValid(GameHUDInstance))
	{
		switch (Ability)
		{
			case EAbilities::Pow:
				GameHUDInstance->AbilityIconPow->SetVisibility(ESlateVisibility::Visible);
				break;
			case EAbilities::Sturdy:
				GameHUDInstance->AbilityIconSturdy->SetVisibility(ESlateVisibility::Visible);
				break;
			default:
				UE_LOG(LogTemp, Error, TEXT("Unknown Ability") );
				break;
		}
	}
}



