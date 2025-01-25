// Copyright Epic Games, Inc. All Rights Reserved.

#include "GGJProjectCharacter.h"

#include "Engine/LocalPlayer.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "HoverButtonWidget.h"
#include "InputActionValue.h"
#include "GGJProject/Core/GGJProjectGameMode.h"
#include "Camera/CameraComponent.h"
#include "Component/SuperellipseOrbitComponent.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

AGGJProjectCharacter::AGGJProjectCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; 
	CameraBoom->bUsePawnControlRotation = false;
	
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
}

void AGGJProjectCharacter::BeginPlay()
{
	Super::BeginPlay();
	AGameModeBase* GameMode = UGameplayStatics::GetGameMode(this);
	if (GameMode)
	{
		USuperellipseOrbitComponent* OrbitComp = GameMode->FindComponentByClass<USuperellipseOrbitComponent>();
		if (OrbitComp)
		{
			OrbitComponent = OrbitComp;
			if (OrbitComponent->bHasBeenInitialized)
			{
				OnOrbitReady();
			}
			else
				if (!bDelegateRegistered)
				{
					OrbitComponent->OnOrbitInitialized.AddDynamic(this, &AGGJProjectCharacter::OnOrbitReady);
					bDelegateRegistered = true;
				}
		}
	}
}

void AGGJProjectCharacter::OnOrbitReady()
{
	FVector DirectionToCurrent = GetActorLocation() - OrbitComponent->CenterLocation;
	DirectionToCurrent.Z = 0.0f;
	CurrentAngle = FMath::Atan2(DirectionToCurrent.Y, DirectionToCurrent.X);
	CurrentAngle = FMath::Fmod(CurrentAngle + 2.0f * PI, 2.0f * PI);
	UE_LOG(LogTemp, Warning, TEXT("CurrentAngle %f"), CurrentAngle);
	bIsOrbitInitialized = true;
	UpdateCameraPosition();
}

void AGGJProjectCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();
	
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AGGJProjectCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AGGJProjectCharacter::Move);
		

		// Looking
		EnhancedInputComponent->BindAction(PauseMenuAction, ETriggerEvent::Started, this, &AGGJProjectCharacter::PauseMenu);

		// Looking
		EnhancedInputComponent->BindAction(CooldownTestAction, ETriggerEvent::Started, this, &AGGJProjectCharacter::CooldownTest);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AGGJProjectCharacter::Move(const FInputActionValue& Value)
{
	if(!bIsOrbitInitialized) return;
	
	FVector2D MovementVector = Value.Get<FVector2D>();
	if (OrbitComponent && OrbitComponent->CenterActor && MovementVector.X != 0.0f)
	{
		float DeltaAngle = FMath::DegreesToRadians(90.0f) * MovementVector.X * GetWorld()->GetDeltaSeconds();
		
		CurrentAngle += DeltaAngle;
		
		CurrentAngle = FMath::Fmod(CurrentAngle, 2.0f * PI);
		if (CurrentAngle < 0.0f)
			CurrentAngle += 2.0f * PI;
		
		FVector2D NewLocation = OrbitComponent->CalculatePosition(CurrentAngle);
		FVector TangentDirection = OrbitComponent->GetTangentDirection(CurrentAngle, FMath::Sign(MovementVector.X));
		
		SetActorLocation(FVector(NewLocation.X, NewLocation.Y, GetActorLocation().Z));
		SetActorRotation(TangentDirection.Rotation());
		UpdateCameraPosition();
	}
}

void AGGJProjectCharacter::UpdateCameraPosition()
{
	if (CameraBoom && OrbitComponent)
	{
		FVector DirectionToCenterOfOrbit = OrbitComponent->CenterLocation - CameraBoom->GetComponentLocation();
		DirectionToCenterOfOrbit.Z = 0;
		FRotator LookAtCenterRotation = DirectionToCenterOfOrbit.Rotation();
		CameraBoom->SetWorldRotation(LookAtCenterRotation);
	}
}

void AGGJProjectCharacter::PauseMenu(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("PauseMenu") );
	bIsPauseMenuVisible = !bIsPauseMenuVisible;

	if (!bIsPauseMenuVisible)
	{
		if (IsValid(HoverButtonWidget))
		{
			HoverButtonWidget->RemoveFromParent();
			AGGJProjectGameMode* GameMode{ Cast<AGGJProjectGameMode>(GetWorld()->GetAuthGameMode())};
			if (IsValid(GameMode))
			{
				GameMode->ResumeGame();
			}
		}
		// todo logic to destroy widget
		return;
	}

	HoverButtonWidget = CreateWidget<UHoverButtonWidget>(GetWorld(), PauseMenuWidgetClass);
	if (IsValid(HoverButtonWidget))
	{
		UE_LOG(LogTemp, Warning, TEXT("Creating widget") );
		HoverButtonWidget->AddToViewport();
		AGGJProjectGameMode* GameMode{ Cast<AGGJProjectGameMode>(GetWorld()->GetAuthGameMode())};
		if (IsValid(GameMode))
		{
			GameMode->PauseGame();
		}
	}
	// todo logic to create widget
	
}

void AGGJProjectCharacter::CooldownTest(const FInputActionValue& Value)
{
	OnBubblePowDelegate.Broadcast();
}
