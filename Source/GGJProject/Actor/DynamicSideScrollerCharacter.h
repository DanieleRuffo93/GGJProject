// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "NiagaraComponent.h"


#include "DynamicSideScrollerCharacter.generated.h"

class UHoverButtonWidget;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class ASplinePathActor;
struct FInputActionValue;
class USuperellipseOrbitComponent;

DECLARE_LOG_CATEGORY_EXTERN(LogPlayerCharacter, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSolidBubbleSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCloudBubbleSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlugBubbleSignature);

UCLASS(config = Game)
class ADynamicSideScrollerCharacter : public ACharacter
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effects", meta = (AllowPrivateAccess = "true"))
	UNiagaraComponent* FeetVFX;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* PauseMenuAction;


public:
	ADynamicSideScrollerCharacter();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Path)
	ASplinePathActor* SplinePath;
	
	UFUNCTION(BlueprintCallable, Category = Path)
	void SetSplinePathActor(ASplinePathActor* NewSplineActor);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Path)
	float ScanDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Jump)
	bool bCanJump ;
	
	virtual void Jump() override;

	virtual void Landed(const FHitResult& Hit) override;


	UFUNCTION(BlueprintCallable, Category = "Jump Buffer")
	void BufferJump();

	UFUNCTION(BlueprintNativeEvent, Category = Character)
	void OnBufferedJump();
	virtual void OnBufferedJump_Implementation();

	UFUNCTION(BlueprintCallable, Category = "Jump Unbuffer")
	void UnbufferJump();

	UFUNCTION(BlueprintNativeEvent, Category = Character)
	void OnUnbufferedJump();
	virtual void OnUnbufferedJump_Implementation();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Jump)
	bool bBufferedJump;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SolidBubble)
	bool bIsSolidBubbleSpawned;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SolidBubble)
	float SolidBubbleCooldown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CloudBubble)
	bool bIsCloudBubbleSpawned;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CloudBubble)
	float CloudBubbleCooldown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlugBubble)
	bool bIsPlugBubbleSpawned;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlugBubble)
	float PlugBubbleCooldown;

	TObjectPtr<USuperellipseOrbitComponent> OrbitComponent;

	float CurrentAngle {0.f};
	bool bIsOrbitInitialized {false};
	bool bDelegateRegistered {false};

	UFUNCTION()
	void OnOrbitReady();
	void UpdateCameraPosition();

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnSolidBubbleSignature OnSolidBubbleDelegate;
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnCloudBubbleSignature OnCloudBubbleDelegate;
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnPlugBubbleSignature OnPlugBubbleDelegate;


protected:
	
	virtual void Tick(float DeltaSeconds) override;
	void Move(const FInputActionValue& Value);
	void MoveSpline(const FInputActionValue& Value);
	void PauseMenu(const FInputActionValue& Value);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "VFX")
	void SpawnFeetVFX();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "VFX")
	void DespawnFeetVFX();
	
	virtual void BeginPlay() override;

	virtual void NotifyControllerChanged() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widget)
	TSubclassOf<UUserWidget> PauseMenuWidgetClass;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UUserWidget> PauseWidget;

private:
	UPROPERTY(VisibleAnywhere)
	bool bIsPauseMenuVisible{ false };
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float SecondsForBufferedJump {0.2};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	float BufferedJumpTimestamp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	bool bIsPlayerJumping{false};

	
};

