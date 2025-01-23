#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GGJProjectGameMode.generated.h"

class USuperellipseOrbitComponent;

UCLASS(minimalapi)
class AGGJProjectGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AGGJProjectGameMode();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orbit")
	TObjectPtr<USuperellipseOrbitComponent> OrbitComponent;

protected:
	virtual void BeginPlay() override;
};



