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



	public:
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			class UDataTable* DT_Weapon = nullptr;
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			class UDataTable* DT_Deputy = nullptr;

		UPROPERTY(BlueprintReadOnly)
		TArray<ABaseCreature*> CreatureUsed;


		UPROPERTY(BlueprintReadOnly)
		class ABaseMonster* CurBoss = nullptr;
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

	public:
		UFUNCTION(BlueprintCallable)
		void AddCreatureUsed(ABaseCreature* Creature);

		UFUNCTION(BlueprintCallable)
		void PopCreatureUsed();

		UFUNCTION(BlueprintCallable)
		ABaseCreature* GetCurCreatureUsed();






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
		
};
