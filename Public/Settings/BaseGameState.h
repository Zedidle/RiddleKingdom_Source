// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Settings/BaseGameManager.h"
#include "BaseGameState.generated.h"


class UBaseGameManager;

UCLASS()
class RPGTUTORIAL_API ABaseGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, Category = "GameManager")
		UBaseGameManager* QuestManager = nullptr;
	UPROPERTY(BlueprintReadOnly, Category = "GameManager")
		UBaseGameManager* MusicManager = nullptr;
	UPROPERTY(BlueprintReadOnly, Category = "GameManager")
		UBaseGameManager* CameraManager = nullptr;


	UPROPERTY(EditAnywhere, Category = "GameManager")
		TSubclassOf<UBaseGameManager> C_QuestManager = nullptr;
	UPROPERTY(EditAnywhere, Category = "GameManager")
		TSubclassOf<UBaseGameManager> C_MusicManager = nullptr;
	UPROPERTY(EditAnywhere, Category = "GameManager")
		TSubclassOf<UBaseGameManager> C_CameraManager = nullptr;

protected:
	virtual void BeginPlay() override;
};
