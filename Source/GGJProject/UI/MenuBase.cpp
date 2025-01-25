#include "MenuBase.h"
#include "Components/Button.h"
#include <Kismet/KismetSystemLibrary.h>
#include <Kismet/GameplayStatics.h>

bool UMenuBase::Initialize()
{
    Super::Initialize();

    return true;
}

void UMenuBase::OnPlayClicked()
{
    UE_LOG(LogTemp, Warning, TEXT("Play Button Clicked"));
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
        UE_LOG(LogTemp, Warning, TEXT("LevelToLoad is not set!"));
    }
}


void UMenuBase::OnExitClicked()
{
    UE_LOG(LogTemp, Warning, TEXT("Exit Button Clicked"));
    UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, true);
}
