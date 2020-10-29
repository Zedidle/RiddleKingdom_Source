// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "IntrudeSpriteAIController.generated.h"

/**
 * 
 */
UCLASS()
class RPGTUTORIAL_API AIntrudeSpriteAIController : public AAIController
{
	GENERATED_BODY()
public:

	virtual void OnPossess(APawn* InPawn) override;

	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

	UFUNCTION(BlueprintImplementableEvent)
		void BP_OnMoveCompleted();

};
