// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "BaseGameState.generated.h"

/**
 * 
 */
UCLASS()
class RPGTUTORIAL_API ABaseGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, Category = "GameManager")
		class ABaseGameManager* QuestManager = nullptr;
	UPROPERTY(BlueprintReadOnly, Category = "GameManager")
		class ABaseGameManager* MusicManager = nullptr;
	UPROPERTY(BlueprintReadOnly, Category = "GameManager")
		class ABaseGameManager* CameraManager = nullptr;


	UPROPERTY(EditAnywhere, Category = "GameManager")
		TSubclassOf<ABaseGameManager> C_QuestManager = nullptr;
	UPROPERTY(EditAnywhere, Category = "GameManager")
		TSubclassOf<ABaseGameManager> C_MusicManager = nullptr;
	UPROPERTY(EditAnywhere, Category = "GameManager")
		TSubclassOf<ABaseGameManager> C_CameraManager = nullptr;


protected:
	virtual void BeginPlay() override;
};
