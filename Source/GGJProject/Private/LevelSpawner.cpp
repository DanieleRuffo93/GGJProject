// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelSpawner.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "Engine/LevelStreamingDynamic.h"
#include "Engine/DataTable.h"

// Sets default values
ALevelSpawner::ALevelSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}



// Called when the game starts or when spawned
void ALevelSpawner::BeginPlay()
{
	Super::BeginPlay();

	UpdateLevelsAndLevelDatas(LevelsDataTable);
	
}

// Called every frame
void ALevelSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALevelSpawner::UpdateLevelsAndLevelDatas(UDataTable* LevelDataTable)
{
	CurrentLevelDatas = NextLevelDatas;
	CurrentLevel = NextLevel;
	bool bLevelFound;
	FName newLevel = ChoseRandomLevelByDifficulty(CurrentDifficulty, LevelDataTable, bLevelFound);
	NextLevelDatas = LevelDataTable->FindRow<FLevelSpawnData>(newLevel, TEXT(""));

	if (CurrentLevelDatas)
	{
		TotalHeight += CurrentLevelDatas->Height;
	}

	LoadLevelByName(FName(NextLevelDatas->LevelAssetName));
}

void ALevelSpawner::LoadLevelByName(FName LevelName)
{
	if (UWorld* World = GetWorld())
	{
		FTransform NewLevelTransform = FTransform(FVector(0, 0, TotalHeight));
		ULevelStreamingDynamic::FLoadLevelInstanceParams Params(GetWorld(), LevelName.ToString(), NewLevelTransform);
		bool bSuccess;
		NextLevel = ULevelStreamingDynamic::LoadLevelInstance(Params, bSuccess);
	}
}

void ALevelSpawner::SpawnNextLevel()
{
	if (LastLevel) LastLevel->SetIsRequestingUnloadAndRemoval(true);
	LastLevel = CurrentLevel;
	CurrentLevel = NextLevel;

	UpdateLevelsAndLevelDatas(LevelsDataTable);
}

FName ALevelSpawner::ChoseRandomLevelByDifficulty(const int32 Difficulty, UDataTable* LevelDataTable, bool& bLevelFound)
{
	TArray<FName> level_names = LevelDataTable->GetRowNames();
	
	TArray<FName> acceptable_levels;

	for (FName level : level_names)
	{
		FLevelSpawnData* potentialLevel = LevelDataTable->FindRow<FLevelSpawnData>(level, TEXT(""));
		if (potentialLevel->Difficulty == Difficulty)
		{
			acceptable_levels.Add(level);
		}
	}
	bLevelFound = acceptable_levels.IsEmpty();

	int32 accepted_level_index = UKismetMathLibrary::RandomIntegerInRange(0, acceptable_levels.Num() - 1);

	return acceptable_levels[accepted_level_index];
}

