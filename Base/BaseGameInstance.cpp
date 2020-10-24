// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGameInstance.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GamePlayStatics.h"
#include "BaseCreature.h"
#include "Blueprint/UserWidget.h"




void UBaseGameInstance::AddCreatureUsed(ABaseCreature* Creature)
{
	UE_LOG(LogTemp, Warning, TEXT("UBaseGameInstance::AddCreatureUsed Creature: %s"), *Creature->GetName());
	if (IsValid(Creature))
	{
		CreatureUsed.RemoveSingle(Creature);
		CreatureUsed.Add(Creature);
	}
}

void UBaseGameInstance::PopCreatureUsed()
{
	UE_LOG(LogTemp, Warning, TEXT("UBaseGameInstance::PopCreatureUsed"));
	CreatureUsed.Pop(false);
}

ABaseCreature* UBaseGameInstance::GetCurCreatureUsed()
{
	if (CreatureUsed.IsValidIndex(CreatureUsed.Num() - 1))
	{
		return CreatureUsed[CreatureUsed.Num() - 1];
	}
	else
	{
		return nullptr;
	}
}


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


void UBaseGameInstance::ShowCreatureHUD(bool bShow)
{
	if (bShow)
	{
		if (IsValid(CreatureHUD))
		{
			CreatureHUD->RemoveFromViewport();
		}
		if (IsValid(C_CreatureHUD))
		{
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

void UBaseGameInstance::ShowCreatureInfo(bool bShow)
{
	if (bShow)
	{
		if (IsValid(CreatureInfoHUD))
		{
			return;
		}
		if (IsValid(C_CreatureInfoHUD))
		{
			CreatureInfoHUD = CreateWidget<UUserWidget>(GetWorld(), C_CreatureInfoHUD);
			CreatureInfoHUD->AddToViewport();
		}
	}
	else
	{
		if (IsValid(CreatureInfoHUD))
		{
			CreatureInfoHUD->RemoveFromViewport();
		}
	}

}


void UBaseGameInstance::ShowEquipInfo(bool bShow)
{
	UE_LOG(LogTemp, Warning, TEXT("UBaseGameInstance::ShowEquipInfo"));
	if (bShow)
	{
		if (IsValid(EquipInfoHUD))
		{
			return;
		}
		if (IsValid(C_EquipInfoHUD))
		{
			EquipInfoHUD = CreateWidget<UUserWidget>(GetWorld(), C_EquipInfoHUD);
			EquipInfoHUD->AddToViewport();
		}
	}
	else
	{
		if (IsValid(EquipInfoHUD))
		{
			EquipInfoHUD->RemoveFromViewport();
		}
	}
}


void UBaseGameInstance::ShowDeadHUD(bool bShow)
{
	UE_LOG(LogTemp, Warning, TEXT("UBaseGameInstance::ShowDeadHUD"));
	if (bShow)
	{
		if (IsValid(DeadHUD))
		{
			DeadHUD->RemoveFromParent();
		}
		if (IsValid(C_DeadHUD))
		{
			DeadHUD = CreateWidget<UUserWidget>(GetWorld(), C_DeadHUD);
			DeadHUD->AddToViewport();
		}
	}
	else
	{
		if (IsValid(DeadHUD))
		{
			DeadHUD->RemoveFromViewport();
		}
	}
}


