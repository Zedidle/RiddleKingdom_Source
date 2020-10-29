// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCreature.h"
#include "BaseMonster.h"
#include "Blueprint/UserWidget.h"
#include "Engine/GameInstance.h"
#include "BaseGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class RPGTUTORIAL_API UBaseGameInstance : public UGameInstance
{
	GENERATED_BODY()

	// SaveData
	public:
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerInfo")
			FString PlayerName = FString("Zedidle");
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerInfo")
			int PlayerIndex = 0;

		UFUNCTION(BlueprintCallable)
			void SetPlayerData(FString Name, int Index);

		UFUNCTION(BlueprintCallable)
			void SaveGame(UBaseSaveGame* SG, FString Name="", int Index = 0);

		UFUNCTION(BlueprintCallable)
			UBaseSaveGame* LoadGame(FString Name="", int Index = -1);


	public:
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			class UDataTable* DT_Weapon = nullptr;
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			class UDataTable* DT_Deputy = nullptr;

		UPROPERTY(BlueprintReadOnly)
		TArray<FString> CreatureUsed;


		UPROPERTY(BlueprintReadOnly)
		class ABaseMonster* CurBoss = nullptr;

		UPROPERTY(BlueprintReadOnly)
		class UUserWidget* ShootAimHUD = nullptr;

		UPROPERTY(BlueprintReadOnly)
		class UUserWidget* BossHealthHUD = nullptr;
		UPROPERTY(BlueprintReadOnly)
		class UUserWidget* CreatureHUD = nullptr;
		UPROPERTY(BlueprintReadOnly)
		class UUserWidget* CreatureInfoHUD = nullptr;
		UPROPERTY(BlueprintReadOnly)
		class UUserWidget* EquipInfoHUD = nullptr;
		UPROPERTY(BlueprintReadOnly)
		class UUserWidget* DeadHUD = nullptr;
		UPROPERTY(BlueprintReadOnly)
		class UUserWidget* KeyDeadHUD = nullptr;
		

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			TSubclassOf<UUserWidget> C_ShootAimHUD = nullptr;
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			TSubclassOf<UUserWidget> C_BossHealth = nullptr;
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			TSubclassOf<UUserWidget> C_CreatureHUD = nullptr;
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			TSubclassOf<UUserWidget> C_CreatureInfoHUD = nullptr;
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			TSubclassOf<UUserWidget> C_EquipInfoHUD = nullptr;
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			TSubclassOf<UUserWidget> C_DeadHUD = nullptr;
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			TSubclassOf<UUserWidget> C_KeyDeadHUD = nullptr;

	public:
		UFUNCTION(BlueprintCallable)
		void AddCreatureUsed(ABaseCreature* Creature);

		UFUNCTION(BlueprintCallable)
		void PopCreatureUsed();

		UFUNCTION(BlueprintCallable)
		ABaseCreature* GetLastCreatureUsed(int Index = 0);

		UFUNCTION(BlueprintCallable, BlueprintPure)
		ABaseCreature* GetCurCreatureUsed();

		
		UFUNCTION(BlueprintCallable)
			void ShowShootAimHUD(bool bShow);

		UFUNCTION(BlueprintCallable)
		void SetCurBoss(ABaseMonster* M);

		UFUNCTION(BlueprintCallable)
		ABaseMonster* GetCurBoss();
		
		UFUNCTION(BlueprintCallable)
		void ShowCreatureHUD(bool bShow);

		UFUNCTION(BlueprintCallable)
		void ShowCreatureInfo(bool bShow);

		UFUNCTION(BlueprintCallable)
		void ShowEquipInfo(bool bShow);

		UFUNCTION(BlueprintCallable)
		void ShowDeadHUD(bool bShow);

		UFUNCTION(BlueprintCallable)
		void ShowKeyDeadHUD(bool bShow);
		
};
