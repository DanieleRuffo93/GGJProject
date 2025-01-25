// Fill out your copyright notice in the Description page of Project Settings.


#include "HoverButtonWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "MediaPlayer.h"
#include "MediaTexture.h"
#include "Components/Border.h"

void UHoverButtonWidget::NativeConstruct()
{
	Super::NativeConstruct();

	
	if (IsValid(HoverButton))
	{
		HoverButton->OnHovered.AddDynamic(this, &UHoverButtonWidget::OnButtonHovered);
		HoverButton->OnUnhovered.AddDynamic(this, &UHoverButtonWidget::OnButtonUnhovered);
	}

	
	if (IsValid(MediaPlayer) && IsValid(MediaTexture) && IsValid(HoverBorder))
	{
		MediaPlayer->SetLooping(true);
		MediaPlayer->SetRate(1.0f);
		HoverBorder->SetBrush(BrushUnhovered);
	}
}

void UHoverButtonWidget::OnButtonHovered()
{
	if (MediaPlayer && FileMediaSource && HoverBorder)
	{
		
		HoverBorder->SetBrush(BrushHover);
		if (MediaPlayer->OpenSource(FileMediaSource))
		{
			UE_LOG(LogTemp, Warning, TEXT("MediaSource successfully opened!"));
			MediaPlayer->SetLooping(true);
			MediaPlayer->SetRate(1.0f);
			MediaPlayer->Play();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to open MediaSource!"));
		}
		
		

	}
}

void UHoverButtonWidget::OnButtonUnhovered()
{
	if (MediaPlayer)
	{
		HoverBorder->SetBrush(BrushUnhovered);
		MediaPlayer->SetLooping(false);
		MediaPlayer->Pause();
	}
}

