// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseSaveGame.h"
#include "Engine/World.h"
#include "../Base/BaseCreature.h"
#include "../Base/BaseGameInstance.h"
#include "Kismet/GameplayStatics.h"

UBaseSaveGame::UBaseSaveGame()
{
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GWorld);
	UBaseGameInstance* GI = Cast<UBaseGameInstance>(GameInstance);
	if (IsValid(GI))
	{
		CreatureUsed = GI->CreatureUsed;
		GameProgress = GI->GameProgress;
		Setting = GI->Setting;
	}
}

UBaseSaveGame* UBaseSaveGame::SaveCreatureData()
{
	UE_LOG(LogTemp, Warning, TEXT("UBaseSaveGame::SavePlayerCreatureData"));

	CreatureDatas.Empty();
	TArray<AActor*> CreatureArray;
	UGameplayStatics::GetAllActorsOfClass(GWorld, ABaseCreature::StaticClass(), CreatureArray);


	UE_LOG(LogTemp, Warning, TEXT("UBaseSaveGame::SavePlayerCreatureData CreatureArray.Num: %d"), CreatureArray.Num());

	for (AActor* Actor : CreatureArray)
	{
		//UE_LOG(LogTemp, Warning, TEXT("UBaseSaveGame::SavePlayerCreatureData 0"));
		ABaseCreature* Creature = Cast<ABaseCreature>(Actor);
		 if (IsValid(Creature))
		 {
			 UE_LOG(LogTemp, Warning, TEXT("UBaseSaveGame::SavePlayerCreatureData Creature Name: %s"), *Creature->GetName());
			 FCreatureData D;
			 D.CreatureName = Creature->GetName();
			 D.bPlayerControlling = Creature->IsPlayerControlled();
			 D.WorldLocation = Creature->GetActorLocation();
			 D.Rotation = Creature->GetActorRotation();
			 D.Faction = Creature->Faction;
			 D.CurHealth = Creature->CurHealth;
			 D.MaxHealth = Creature->MaxHealth;
			 D.bBeenControlled = Creature->bBeenControlled;
			 CreatureDatas.Add(D);
		 }
	}
	return this;
}