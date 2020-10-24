// Fill out your copyright notice in the Description page of Project Settings.


#include "CreatureAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "CreatureBT.h"


void ACreatureAIController::OnPossess(APawn* InPawn)
{
	if (InPawn != nullptr && InPawn->IsPendingKill())
	{
		return;
	}
	Super::OnPossess(InPawn);

	if (IsValid(BTAsset))
	{
		RunBehaviorTree(BTAsset);
	}
}

void ACreatureAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);
	BP_OnMoveCompleted();
}
