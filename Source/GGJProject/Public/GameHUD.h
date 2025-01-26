// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameHUD.generated.h"

class UTextBlock;
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TObjectPtr<ADynamicSideScrollerCharacter> Character;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD", meta = (BindWidget))
	TObjectPtr<UTextBlock> Tutoring;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	FText TutoringTextPow;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	FText TutoringTextSturdy;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	float HideTutorialTextAfter {1.f};

protected:

	

	virtual void NativeConstruct() override;

private:
	

	bool bIsAlreadyInitialized {false};
	
};
