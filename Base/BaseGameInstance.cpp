// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGameInstance.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GamePlayStatics.h"
#include "BaseCreature.h"
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
		UE_LOG(LogTemp, Warning, TEXT("UBaseGameInstance::ShowCreatureHUD true"));

		if (IsValid(C_CreatureHUD))
		{
			UE_LOG(LogTemp, Warning, TEXT("UBaseGameInstance::ShowCreatureHUD true 111"));

			if (IsValid(CreatureHUD))
			{
				UE_LOG(LogTemp, Warning, TEXT("UBaseGameInstance::ShowCreatureHUD true 22"));
				CreatureHUD->RemoveFromViewport();
			}
			CreatureHUD = CreateWidget<UUserWidget>(GetWorld(), C_CreatureHUD);
			CreatureHUD->AddToViewport();
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UBaseGameInstance::ShowCreatureHUD false"));

		if (IsValid(CreatureHUD))
		{
			
			CreatureHUD->RemoveFromViewport();
		}
	}
}


