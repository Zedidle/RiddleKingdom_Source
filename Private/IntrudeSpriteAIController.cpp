// Fill out your copyright notice in the Description page of Project Settings.


#include "IntrudeSpriteAIController.h"



void AIntrudeSpriteAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

}


void AIntrudeSpriteAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);
	BP_OnMoveCompleted();
}
