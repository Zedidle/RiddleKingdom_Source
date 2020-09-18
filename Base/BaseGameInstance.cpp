// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGameInstance.h"

void UBaseGameInstance::SetCurBoss(ABaseMonster* M)
{
	CurBoss = M;
	if (UW_BossHealth)
	{
		UUserWidget* UW = CreateWidget<UUserWidget>(GetWorld(), UW_BossHealth);
		UW->AddToViewport();
	}
}

ABaseMonster* UBaseGameInstance::GetCurBoss()
{
	return CurBoss;
}
