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

	public:
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			TSubclassOf<UUserWidget> C_CreatureHUD = nullptr;


		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			TSubclassOf<UUserWidget> C_BossHealth = nullptr;

	public:
		UFUNCTION(BlueprintCallable)
		void SetCurBoss(ABaseMonster* M);

		UFUNCTION(BlueprintCallable)
		ABaseMonster* GetCurBoss();
		
		UFUNCTION(BlueprintCallable)
		void ShowCreatureHUD(bool IsShow);


};
