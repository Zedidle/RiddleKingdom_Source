// Fill out your copyright notice in the Description page of Project Settings.


#include "Settings/BaseGameState.h"
#include "Settings/BaseGameManager.h"

void ABaseGameState::BeginPlay()
{
	Super::BeginPlay();
	if (!IsValid(MusicManager) && IsValid(C_MusicManager))
	{
		MusicManager = GetWorld()->SpawnActor<UBaseGameManager>(C_MusicManager);
	}
	if (!IsValid(QuestManager) && IsValid(C_QuestManager))
	{
		QuestManager = GetWorld()->SpawnActor<UBaseGameManager>(C_QuestManager);
	}
	if (!IsValid(CameraManager) && IsValid(C_CameraManager))
	{
		CameraManager = GetWorld()->SpawnActor<UBaseGameManager>(C_CameraManager);
	}
}
