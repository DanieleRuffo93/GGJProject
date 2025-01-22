// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/DinamicSideScrollerCharacter.h"

// Sets default values
ADinamicSideScrollerCharacter::ADinamicSideScrollerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADinamicSideScrollerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADinamicSideScrollerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ADinamicSideScrollerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

