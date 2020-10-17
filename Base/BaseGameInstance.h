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

	class ABaseMonster* CurBoss = nullptr;
	
	class UUserWidget* BossHealthHUD = nullptr;

	class UUserWidget* CreatureHUD = nullptr;

	class UUserWidget* CreatureInfoHUD = nullptr;

	class UUserWidget* EquipInfoHUD = nullptr;

	public:
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			TSubclassOf<UUserWidget> C_BossHealth = nullptr;
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			TSubclassOf<UUserWidget> C_CreatureHUD = nullptr;
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			TSubclassOf<UUserWidget> C_CreatureInfoHUD = nullptr;
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			TSubclassOf<UUserWidget> C_EquipInfoHUD = nullptr;


	public:
		UFUNCTION(BlueprintCallable)
		void SetCurBoss(ABaseMonster* M);

		UFUNCTION(BlueprintCallable)
		ABaseMonster* GetCurBoss();
		
		UFUNCTION(BlueprintCallable)
		void ShowCreatureHUD(bool bShow);

		UFUNCTION(BlueprintCallable)
		void ShowCreatureInfo(bool bShow);

		UFUNCTION(BlueprintCallable)
			void ShowEquipInfo();

};
