// Fill out your copyright notice in the Description page of Project Settings.

#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

#include "ActorComponents/BaseAttributeSet.h"
#include "Settings/BaseSaveGame.h"
#include "Pawns/BaseCreature.h"
#include "Settings/BaseGameInstance.h"



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
			 //UE_LOG(LogTemp, Warning, TEXT("UBaseSaveGame::SavePlayerCreatureData Creature Name: %s"), *Creature->GetName());
			 FCreatureData D;
			 D.CreatureName = Creature->GetName();
			 D.bPlayerControlling = Creature->IsPlayerControlled();
			 if (D.bPlayerControlling)
			 {
				 UE_LOG(LogTemp, Warning, TEXT("UBaseSaveGame::SavePlayerCreatureData Creature Name: %s"), *Creature->GetName());
				 UE_LOG(LogTemp, Warning, TEXT("UBaseSaveGame::SavePlayerCreatureData Creature bPlayerControlling"));
			 }
			 D.WorldLocation = Creature->GetActorLocation();
			 D.Rotation = Creature->GetActorRotation();
			 D.Faction = Creature->Faction;
			 D.BaseAttributeSet = Creature->BaseAttributeSet;
			 D.bBeenControlled = Creature->bBeenControlled;
			 CreatureDatas.Add(D);
		 }
	}
	return this;
}