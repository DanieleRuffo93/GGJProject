// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "DynamicSideScrollerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class ASplinePathActor;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogPlayerCharacter, Log, All);

UCLASS(config = Game)
class ADynamicSideScrollerCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;


public:
	ADynamicSideScrollerCharacter();

	/** Spline path */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Path)
	ASplinePathActor* SplinePath;

	/** Set spline path */
	UFUNCTION(BlueprintCallable, Category = Path)
	void SetSplinePathActor(ASplinePathActor* NewSplineActor);

	/** Distance from spline */
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


protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);


protected:

	virtual void NotifyControllerChanged() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

