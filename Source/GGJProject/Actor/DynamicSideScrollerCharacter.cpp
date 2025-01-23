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
#include "Component/SuperellipseOrbitComponent.h"
#include "GGJProject/Public/Actor/SplinePathActor.h"
#include "Components/SplineComponent.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY(LogPlayerCharacter);


ADynamicSideScrollerCharacter::ADynamicSideScrollerCharacter()
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
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	
	ScanDistance = 250.0f;
	bCanJump = false;

	bBufferedJump = false;

	bIsSolidBubbleSpawned = false;
	SolidBubbleCooldown = 1.0f;
}

void ADynamicSideScrollerCharacter::NotifyControllerChanged()
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

void ADynamicSideScrollerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ADynamicSideScrollerCharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		
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

void ADynamicSideScrollerCharacter::BeginPlay()
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
					OrbitComponent->OnOrbitInitialized.AddDynamic(this, &ADynamicSideScrollerCharacter::OnOrbitReady);
					bDelegateRegistered = true;
				}
		}
	}
}

void ADynamicSideScrollerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	UpdateCameraPosition();
	if(bDrawDebug)
	{
		OrbitComponent->DrawOrbit(GetActorLocation().Z);
	}
}

void ADynamicSideScrollerCharacter::OnOrbitReady()
{
	FVector DirectionToCurrent = GetActorLocation() - OrbitComponent->CenterLocation;
	DirectionToCurrent.Z = 0.0f;
	CurrentAngle = FMath::Atan2(DirectionToCurrent.Y, DirectionToCurrent.X);
	CurrentAngle = FMath::Fmod(CurrentAngle + 2.0f * PI, 2.0f * PI);
	UE_LOG(LogTemp, Warning, TEXT("CurrentAngle %f"), CurrentAngle);
	bIsOrbitInitialized = true;
	UpdateCameraPosition();
	
}

void ADynamicSideScrollerCharacter::MoveSpline(const FInputActionValue& Value)
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

		// add movement ;
		AddMovementInput(RightDirection, FMath::Abs(movement));
	}
}


void ADynamicSideScrollerCharacter::Move(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Input: %f"), Value.Get<float>())
	if (!bIsOrbitInitialized || !OrbitComponent) return;

	float DeltaX = GetActorLocation().X - OrbitComponent->CenterLocation.X;
	float DeltaY = GetActorLocation().Y - OrbitComponent->CenterLocation.Y;

	CurrentAngle = FMath::Atan2(DeltaY, DeltaX);
	
	float InputValue = Value.Get<float>();
	if (InputValue == 0.0f) return;
	
	int8 MovementDirection = (InputValue > 0.0f) ? 1 : -1;
	
	CurrentAngle = OrbitComponent->CalculateDeltaAngle(CurrentAngle, GetWorld()->GetDeltaSeconds(), GetCharacterMovement()->MaxWalkSpeed, MovementDirection);
	
	FVector2D NextPosition2D = OrbitComponent->CalculatePosition(CurrentAngle);
	FVector NextPosition(NextPosition2D.X, NextPosition2D.Y, GetActorLocation().Z);
	
	FVector MovementDirectionVector = (NextPosition - GetActorLocation()).GetSafeNormal();
	
	AddMovementInput(MovementDirectionVector);
	
}

void ADynamicSideScrollerCharacter::UpdateCameraPosition()
{
	if (CameraBoom && OrbitComponent)
	{
		FVector DirectionToCenterOfOrbit = OrbitComponent->CenterLocation - CameraBoom->GetComponentLocation();
		DirectionToCenterOfOrbit.Z = 0;
		FRotator LookAtCenterRotation = DirectionToCenterOfOrbit.Rotation();
		CameraBoom->SetWorldRotation(LookAtCenterRotation);
	}
}