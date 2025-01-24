// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/LevelStreamingDynamic.h"
#include "LevelSpawner.generated.h"

USTRUCT(BlueprintType)
struct GGJPROJECT_API FLevelSpawnData : public FTableRowBase
{
    GENERATED_BODY()

public:
    // Nome descrittivo del livello (facoltativo)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Difficulty;

    // Riferimento al livello (Soft Object Path)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Height;

    // Altri dati utili per i tuoi scopi
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString LevelAssetName;
};


UCLASS()
class GGJPROJECT_API ALevelSpawner : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Datas")
	UDataTable* LevelsDataTable;
public:	
	// Sets default values for this actor's properties
	ALevelSpawner();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Levels")
	ULevelStreamingDynamic* NextLevel;						
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Levels")
	ULevelStreamingDynamic* CurrentLevel;					
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Levels")
	ULevelStreamingDynamic* LastLevel;

	FLevelSpawnData* NextLevelDatas;
	FLevelSpawnData* CurrentLevelDatas;
	FLevelSpawnData* LastLevelDatas;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Levels")
	int32 CurrentDifficulty;
	float TotalHeight;

private:
	virtual void UpdateLevelsAndLevelDatas(UDataTable* LevelDataTable);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual FName ChoseRandomLevelByDifficulty(const int32 Difficulty, UDataTable* LevelDataTable, bool& bLevelFound);

	void LoadLevelByName(FName LevelName);

	UFUNCTION(BlueprintCallable)
	virtual void SpawnNextLevel();

};
