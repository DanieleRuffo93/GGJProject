// Fill out your copyright notice in the Description page of Project Settings.


#include "GameHUD.h"

#include "AbilityIcon.h"
#include "Components/Image.h"
#include "GGJProject/Actor/DynamicSideScrollerCharacter.h"
#include "GGJProject/Actor/GGJProjectCharacter.h"


void UGameHUD::NativeConstruct()
{
	Super::NativeConstruct();

	Character = Cast<ADynamicSideScrollerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (IsValid(Character))
	{
		if (!bIsAlreadyInitialized)
		{
			InitializeHUD();
		}
	}

}
