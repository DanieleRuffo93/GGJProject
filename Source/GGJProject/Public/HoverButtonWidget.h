// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HoverButtonWidget.generated.h"

class UBorder;
class UMediaSource;
class UFileMediaSource;
class UMediaTexture;
class UMediaPlayer;
class UTextBlock;
class UButton;

UCLASS()
class GGJPROJECT_API UHoverButtonWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> HoverBorder;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSlateBrush BrushHover;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSlateBrush BrushUnhovered;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> HoverButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ButtonText;

	// Media Player per gestire il video
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Media")
	TObjectPtr<UMediaPlayer> MediaPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Media")
	TObjectPtr<UMediaTexture> MediaTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Media")
	TObjectPtr<UMediaSource> FileMediaSource;

private:
	UFUNCTION()
	void OnButtonHovered();

	UFUNCTION()
	void OnButtonUnhovered();
	
};
