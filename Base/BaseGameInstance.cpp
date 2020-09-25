// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGameInstance.h"
#include "Blueprint/UserWidget.h"


void UBaseGameInstance::SetCurBoss(ABaseMonster* M)
{
	UE_LOG(LogTemp, Warning, TEXT(" UBaseGameInstance::SetCurBoss"));
	if (IsValid(nullptr))
	{
		UE_LOG(LogTemp, Warning, TEXT(" UBaseGameInstance::SetCurBoss, nullptr is Valid"));
	}

	if (IsValid(M))
	{
		UE_LOG(LogTemp, Warning, TEXT("UBaseGameInstance::SetCurBoss Valid"));
		CurBoss = M;
		if (C_BossHealth)
		{
			BossHealthHUD = CreateWidget<UUserWidget>(GetWorld(), C_BossHealth);
			BossHealthHUD->AddToViewport();
		}
	}
	else
	{
		if (IsValid(BossHealthHUD))
		{
			UE_LOG(LogTemp, Warning, TEXT("BossHealthHUD->RemoveFromViewport"));
			BossHealthHUD->RemoveFromViewport();
		}
	}
}

ABaseMonster* UBaseGameInstance::GetCurBoss()
{
	return CurBoss;
}


void UBaseGameInstance::ShowCreatureHUD(bool IsShow)
{
	if (IsShow)
	{
		if (IsValid(C_CreatureHUD))
		{
			if (IsValid(CreatureHUD))
			{
				CreatureHUD->RemoveFromViewport();
			}
			CreatureHUD = CreateWidget<UUserWidget>(GetWorld(), C_CreatureHUD);
			CreatureHUD->AddToViewport();
		}
	}
	else
	{
		if (IsValid(CreatureHUD))
		{
			
			CreatureHUD->RemoveFromViewport();
		}
	}
}


