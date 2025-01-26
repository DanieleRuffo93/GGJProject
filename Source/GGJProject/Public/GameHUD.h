// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameHUD.generated.h"

class ADynamicSideScrollerCharacter;
class AGGJProjectCharacter;
class UAbilityIcon;
/**
 * 
 */
UCLASS()
class GGJPROJECT_API UGameHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
	void InitializeHUD();

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), category = "HUD", EditDefaultsOnly)
	TObjectPtr<UAbilityIcon> AbilityIconSturdy;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), category = "HUD", EditDefaultsOnly)
	TObjectPtr<UAbilityIcon> AbilityIconPow;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), category = "HUD", EditDefaultsOnly)
	TObjectPtr<UAbilityIcon> AbilityIconBouncy;

protected:

	

	virtual void NativeConstruct() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess=true), Category = "HUD")
	TObjectPtr<ADynamicSideScrollerCharacter> Character;

	bool bIsAlreadyInitialized {false};
	
};
