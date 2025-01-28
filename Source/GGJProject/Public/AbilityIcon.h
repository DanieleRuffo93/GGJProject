// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AbilityIcon.generated.h"

class UProgressBar;
class UImage;
/**
 * 
 */
UCLASS()
class GGJPROJECT_API UAbilityIcon : public UUserWidget
{
	GENERATED_BODY()

public:
	// Variabili per immagine, cooldown, ecc.
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> AbilityImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTexture> AbilityTexture;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UMaterialInterface> MaterialInterface;

	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> MaterialInstance;
	
	UFUNCTION(BlueprintCallable)
	void StartCooldown();

	UFUNCTION(BlueprintCallable)
	void UpdateCooldown();

	UFUNCTION(BlueprintCallable)
	FSlateBrush GetBrush();

private:
	FTimerHandle CooldownTimerHandle;
	float CurrentTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float CooldownTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FSlateBrush ImageBrush;
	bool bIsOnCooldown{false};

	virtual void NativeConstruct() override;
};
