// Copyright Epic Games, Inc. All Rights Reserved.

#include "DynamicSideScrollerCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "GGJProject/Public/Actor/SplinePathActor.h"
#include "Components/SplineComponent.h"

DEFINE_LOG_CATEGORY(LogPlayerCharacter);


ADynamicSideScrollerCharacter::ADynamicSideScrollerCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = false; 

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	ScanDistance = 250.0f;
	bCanJump = false;

	bBufferedJump = false;

	bIsSolidBubbleSpawned = false;
	SolidBubbleCooldown = 1.0f;

	bIsCloudBubbleSpawned = false;
	CloudBubbleCooldown = 1.0f;
}

//////////////////////////////////////////////////////////////////////////
// Input

void ADynamicSideScrollerCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void ADynamicSideScrollerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ADynamicSideScrollerCharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADynamicSideScrollerCharacter::Move);
	}
	else
	{
		UE_LOG(LogPlayerCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ADynamicSideScrollerCharacter::Jump()
{
	if (!CanJump())
	{
		BufferJump();
		return;
	}

	Super::Jump();

	if (bCanJump)
	{
		FVector LaunchVector(0.0f, 0.0f, GetCharacterMovement()->JumpZVelocity);
		LaunchCharacter(LaunchVector, false, false);
	}
}

void ADynamicSideScrollerCharacter::BufferJump()
{
	bBufferedJump = true;
}

void ADynamicSideScrollerCharacter::OnBufferedJump_Implementation()
{
	BufferJump();
}

void ADynamicSideScrollerCharacter::UnbufferJump()
{
	bBufferedJump = false;
}

void ADynamicSideScrollerCharacter::OnUnbufferedJump_Implementation()
{
	UnbufferJump();
}

void ADynamicSideScrollerCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

}

void ADynamicSideScrollerCharacter::Move(const FInputActionValue& Value)
{
	// input is a float

	float movement = Value.Get<float>();

	if (movement > 0)
	{
		movement = 1.0f;
	}
	else if (movement < 0)
	{
		movement = -1.0f;
	}


	if (Controller != nullptr && SplinePath != nullptr)
	{

		//return the perpendicular projection (tangent) of Character's X coordinates on the spline
		FVector Tangent = SplinePath->Spline->FindTangentClosestToWorldLocation(GetActorLocation(), ESplineCoordinateSpace::World);
		Tangent.Normalize();

		//A Scaled vector representing tangent on the spline direction 
		FVector ScaledTangent = Tangent * ScanDistance;

		//Convert tangent to float
		FVector TangentBasedOnMovement = ScaledTangent * movement;

		// get right vector 
		FVector RightDirection = SplinePath->Spline->FindLocationClosestToWorldLocation(TangentBasedOnMovement + GetActorLocation(), ESplineCoordinateSpace::World);
		RightDirection -= GetActorLocation();
		RightDirection.Normalize();

		SetActorRotation(FRotator(0.0f, RightDirection.Rotation().Yaw, 0.0f));

		// add movement ;
		AddMovementInput(RightDirection, FMath::Abs(movement));
	}
}

void ADynamicSideScrollerCharacter::SetSplinePathActor(ASplinePathActor* NewSplinePathActor)
{
	if (IsValid(NewSplinePathActor) && NewSplinePathActor != SplinePath) //Check if new actor is valid and not the current
	{
		SplinePath = NewSplinePathActor;
		return;
	}
	UE_LOG(LogPlayerCharacter, Warning, TEXT("Spline Path not assigned or already present."));
}


