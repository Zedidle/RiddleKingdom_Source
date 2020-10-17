// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CreatureAIController.generated.h"

/**
 * 
 */
UCLASS()
class RPGTUTORIAL_API ACreatureAIController : public AAIController
{
	GENERATED_BODY()
public:
	UPROPERTY(Category = BehaviorTree, EditAnywhere, BlueprintReadWrite)
		class UBehaviorTree* BTAsset = nullptr;


	virtual void OnPossess(APawn* InPawn) override;
};
