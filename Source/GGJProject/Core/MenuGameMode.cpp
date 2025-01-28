// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuGameMode.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraActor.h"
#include "Kismet/GameplayStatics.h"

void AMenuGameMode::BeginPlay()
{
    if (!IsValid(MenuWidgetClass) )
    {
        return;
    }
    TArray<AActor*> FoundCameras;
    UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), ACameraActor::StaticClass(), FName("Menu"), FoundCameras);
    if (FoundCameras.Num() > 0)
    {
       CurrentCamera = Cast<ACameraActor>(FoundCameras[0]);
    }
    if (!IsValid(CurrentCamera))
    {
        UE_LOG(LogTemp, Error, TEXT("Camera not valid!! Spawning") );
        FActorSpawnParameters SpawnParameters;
        SpawnParameters.Owner = this;
        SpawnParameters.Instigator = GetInstigator();
        CurrentCamera = GetWorld()->SpawnActor<ACameraActor>(CameraActorToSpawn, CameraSpawnLocation, CameraSpawnRotation, SpawnParameters);
    }
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
    
    
    CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), MenuWidgetClass);
    if (CurrentWidget && PlayerController)
    {
        PlayerController->SetViewTarget(CurrentCamera);
        PlayerController->bShowMouseCursor = true;
        PlayerController->SetInputMode(FInputModeUIOnly());
        CurrentWidget->AddToViewport();
    }
}
