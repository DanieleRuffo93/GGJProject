#include "BFL_Spawn.h"
#include "Engine/LevelStreamingDynamic.h"

void UBFL_Spawn::SpawnLevelAtLocation(UObject* WorldContextObject, const FString & LevelName, const FVector& Location, const FRotator & Rotation)
{
    if (!WorldContextObject)
    {
        UE_LOG(LogTemp, Warning, TEXT("Il mondo non è valido!"));
        return;
    }

    UWorld* World = WorldContextObject->GetWorld();

    if (World)
    {
        UE_LOG(LogTemp, Warning, TEXT("Il mondo non è valido!"))
            return;
    }

    bool bOutSuccess = false; // Variabile per verificare se il caricamento ha successo
    ULevelStreamingDynamic* LevelInstance = ULevelStreamingDynamic::LoadLevelInstance(
        World,                       // Oggetto di contesto
        LevelName,                   // Nome del livello
        Location,                    // Posizione nel mondo
        Rotation,                    // Rotazione
        bOutSuccess                  // Successo del caricamento
    );

    if (LevelInstance && bOutSuccess)
    {
        UE_LOG(LogTemp, Log, TEXT("Livello %s caricato con successo a posizione %s"), *LevelName, *Location.ToString());
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Errore nel caricamento del livello %s"), *LevelName);
    }
}
