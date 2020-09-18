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

	public:
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			TSubclassOf<UUserWidget> UW_BossHealth = nullptr;

	public:
		UFUNCTION(BlueprintCallable)
		void SetCurBoss(ABaseMonster* M);
		UFUNCTION(BlueprintCallable)
		ABaseMonster* GetCurBoss();

};
