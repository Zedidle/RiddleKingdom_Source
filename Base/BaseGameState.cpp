// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGameState.h"
#include "BaseGameManager.h"

void ABaseGameState::BeginPlay()
{
	Super::BeginPlay();
	if (!IsValid(MusicManager) && IsValid(C_MusicManager))
	{
		UE_LOG(LogTemp, Warning, TEXT("UBaseGameInstance::UBaseGameInstance SpawnMusicManager"));
		MusicManager = GetWorld()->SpawnActor<ABaseGameManager>(C_MusicManager);
	}
	if (!IsValid(QuestManager) && IsValid(C_QuestManager))
	{
		UE_LOG(LogTemp, Warning, TEXT("UBaseGameInstance::UBaseGameInstance SpawnQuestManager"));
		QuestManager = GetWorld()->SpawnActor<ABaseGameManager>(C_QuestManager);
	}
	if (!IsValid(CameraManager) && IsValid(C_CameraManager))
	{
		UE_LOG(LogTemp, Warning, TEXT("UBaseGameInstance::UBaseGameInstance SpawnCameraManager"));
		CameraManager = GetWorld()->SpawnActor<ABaseGameManager>(C_CameraManager);
	}
}
