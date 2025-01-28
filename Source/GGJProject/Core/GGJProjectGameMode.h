#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GGJProjectGameMode.generated.h"

class UGameHUD;
class USuperellipseOrbitComponent;

UENUM(BlueprintType)
enum EAbilities : uint8
{
	Pow UMETA(DisplayName = "Pow"),
	Sturdy UMETA(DisplayName = "Sturdy")
};

UCLASS(MinimalAPI)
class AGGJProjectGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menu", meta = (AllowedClasses = "World"))
	TSoftObjectPtr<UWorld> LevelToLoad;




public:
	AGGJProjectGameMode();

	UFUNCTION(BlueprintCallable, Category = "Pause")
	void PauseGame();

	UFUNCTION(BlueprintCallable, Category = "Pause")
	void ResumeGame();

	UFUNCTION(BlueprintCallable, Category = "Pause")
	void QuitGame();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widget)
	TSubclassOf<UUserWidget> PauseMenuWidgetClass;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UUserWidget> PauseWidget;
	UFUNCTION(BlueprintCallable)
	void EnableAbility(const EAbilities Ability);
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UUserWidget> GameHUD;
	

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> GameHUDClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orbit", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USuperellipseOrbitComponent> OrbitComponent;

	

	
};



