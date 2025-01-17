#include "MenuBase.h"
#include "Components/Button.h"
#include <Kismet/KismetSystemLibrary.h>
#include <Kismet/GameplayStatics.h>

bool UMenuBase::Initialize()
{
    Super::Initialize();

    if (PlayButton)
        PlayButton->OnClicked.AddDynamic(this, &UMenuBase::OnPlayClicked);

    if (OptionsButton)
        OptionsButton->OnClicked.AddDynamic(this, &UMenuBase::OnOptionsClicked);

    if (ExitButton)
        ExitButton->OnClicked.AddDynamic(this, &UMenuBase::OnExitClicked);

    return true;
}

void UMenuBase::OnPlayClicked()
{
    UE_LOG(LogTemp, Warning, TEXT("Play Button Clicked"));
    if (LevelToLoad.IsValid() || LevelToLoad.IsPending())
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

void UMenuBase::OnOptionsClicked()
{
    UE_LOG(LogTemp, Warning, TEXT("Options Button Clicked"));
    // Logic to open options
}

void UMenuBase::OnExitClicked()
{
    UE_LOG(LogTemp, Warning, TEXT("Exit Button Clicked"));
    UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, true);
}
