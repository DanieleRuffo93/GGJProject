// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityIcon.h"

#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "GGJProject/Actor/GGJProjectCharacter.h"

void UAbilityIcon::StartCooldown()
{
	UE_LOG(LogTemp, Warning, TEXT("EUREKAAA") );
	bIsOnCooldown = true;
	CurrentTime = 0.01;
	
	// CooldownProgressBar->SetVisibility(ESlateVisibility::Visible);
    
	GetWorld()->GetTimerManager().SetTimer(
		CooldownTimerHandle,
		this,
		&UAbilityIcon::UpdateCooldown,
		0.1f,
		true
	);
}

void UAbilityIcon::UpdateCooldown()
{
	if (CurrentTime <= CooldownTime)
	{
		CurrentTime += 0.1f;
		float Progress = 1.8f - (CurrentTime / CooldownTime)*1.8;
		if (MaterialInstance)
		{
		
			MaterialInstance->SetScalarParameterValue(FName("Progress"), Progress);
		}
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(CooldownTimerHandle);
		bIsOnCooldown = false;

		// Resetta il parametro
		if (MaterialInstance)
		{
			MaterialInstance->SetScalarParameterValue(FName("Progress"), 0.0f);
		}
	}
}

FSlateBrush UAbilityIcon::GetBrush()
{
	return ImageBrush;
}

void UAbilityIcon::NativeConstruct()
{
	Super::NativeConstruct();

	UE_LOG(LogTemp, Warning, TEXT("Constructed") );
	

	
	if (AbilityImage)
	{
		UE_LOG(LogTemp, Warning, TEXT("Qui") );
		if (MaterialInterface)
		{
			UE_LOG(LogTemp, Warning, TEXT("Qui2") );

			// Crea una Material Instance dinamica basata su quella statica
			MaterialInstance = UMaterialInstanceDynamic::Create(MaterialInterface, this);
			MaterialInstance->SetTextureParameterValue(FName("ImageTexture"), AbilityTexture);

			// Imposta la Material Instance dinamica sull'immagine
			AbilityImage->SetBrushFromMaterial(MaterialInstance);
		}
	}
}
