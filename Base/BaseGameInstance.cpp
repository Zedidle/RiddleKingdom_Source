// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGameInstance.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GamePlayStatics.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "AIController.h"
#include "BaseCreature.h"
#include "../public/BaseSaveGame.h"
#include "Blueprint/UserWidget.h"



void UBaseGameInstance::SetPlayerData(FString Name, int Index)
{
	PlayerName = Name;
	PlayerIndex = Index;
}

void UBaseGameInstance::SaveGame(UBaseSaveGame* SG, FString Name, int Index)
{
	UE_LOG(LogTemp, Warning, TEXT("UBaseGameInstance::SaveGame"));
	if (Name == "")
	{
		Name = PlayerName;
	}
	if (Index < 0)
	{
		Index = PlayerIndex;
	}

	UGameplayStatics::SaveGameToSlot(SG, Name, Index);
}

UBaseSaveGame* UBaseGameInstance::LoadGame(FString Name, int Index)
{
	UE_LOG(LogTemp, Warning, TEXT("UBaseGameInstance::LoadGame"));
	if (Name == "")
	{
		Name = PlayerName;
	}
	if (Index == -1)
	{
		Index = PlayerIndex;
	}

	USaveGame* SavaGame = UGameplayStatics::LoadGameFromSlot(Name, Index);
	UBaseSaveGame* SG = Cast<UBaseSaveGame>(SavaGame);

	TArray<AActor*> CreatureArray;
	UGameplayStatics::GetAllActorsOfClass(GWorld, ABaseCreature::StaticClass(), CreatureArray);

	if (IsValid(SG))
	{

		// 夺舍链
		CreatureUsed = SG->CreatureUsed;


		// 复原所有在场 Creature
		UE_LOG(LogTemp, Warning, TEXT("UBaseGameInstance::LoadGame Creature 0"));
		TMap<FString, FCreatureData> CreatureNameToDatas;
		for (FCreatureData D : SG->CreatureDatas)
		{
			CreatureNameToDatas.Add(D.CreatureName, D);
		}

		for (AActor* Creature : CreatureArray)
		{
			ABaseCreature* C = Cast<ABaseCreature>(Creature);
			if (IsValid(C))
			{
				FString Name = C->GetName();
				if (CreatureNameToDatas.Contains(Name))
				{
					
					FCreatureData D = CreatureNameToDatas[Name];
					APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
					C->SetActorLocation(D.WorldLocation);
					C->SetActorRotation(D.Rotation);
					C->Faction = D.Faction;
					C->CurHealth = D.CurHealth;
					C->MaxHealth = D.MaxHealth;
					C->bBeenControlled = D.bBeenControlled;
					C->Target = nullptr;
					C->ResetSave();

					// 控制中的角色继续控制
					if (D.bPlayerControlling)
					{
						if (!C->IsPlayerControlling())
						{
							PlayerController->Possess(C);
						}
					}
					//// 控制过或死亡中的角色，除了原本就是盟友外，失去活动能力
					else if (D.bBeenControlled || D.CurHealth <= 0)
					{
						if (!C->IsPlayerControlling())
						{
							C->Dead(); // 可能导致奔溃
						}
					}
					//// 没有控制过且没死过的
					//else if (D.CurHealth > 0)
					//{
					//	C->Revive();
					//}
				}
			}
		}
	}

	return SG;
}



void UBaseGameInstance::AddCreatureUsed(ABaseCreature* Creature)
{
	UE_LOG(LogTemp, Warning, TEXT("UBaseGameInstance::AddCreatureUsed Creature: %s"), *Creature->GetName());
	if (IsValid(Creature))
	{
		CreatureUsed.RemoveSingle(Creature->GetName());
		CreatureUsed.Add(Creature->GetName());
	}
}

void UBaseGameInstance::PopCreatureUsed()
{
	UE_LOG(LogTemp, Warning, TEXT("UBaseGameInstance::PopCreatureUsed"));
	CreatureUsed.Pop(false);
}

ABaseCreature* UBaseGameInstance::GetLastCreatureUsed(int Index)
{
	TArray<AActor*> CreatureArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseCreature::StaticClass(), CreatureArray);
	int ReverseIndex = CreatureUsed.Num() - 1 - Index;
	if (CreatureUsed.IsValidIndex(ReverseIndex))
	{
		for (AActor* Creature : CreatureArray)
		{
			ABaseCreature* C = Cast<ABaseCreature>(Creature);
			if (IsValid(C) && C->GetName() == CreatureUsed[ReverseIndex])
			{
				return C;
			}
		}
	}
	return nullptr;
}

ABaseCreature* UBaseGameInstance::GetCurCreatureUsed()
{
	return GetLastCreatureUsed();
}


void UBaseGameInstance::SetCurBoss(ABaseMonster* M)
{
	UE_LOG(LogTemp, Warning, TEXT(" UBaseGameInstance::SetCurBoss"));
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


void UBaseGameInstance::ShowShootAimHUD(bool bShow)
{
	if (IsValid(ShootAimHUD))
	{
		ShootAimHUD->RemoveFromViewport();
	}
	if (bShow)
	{
		if (IsValid(C_ShootAimHUD))
		{
			ShootAimHUD = CreateWidget<UUserWidget>(GetWorld(), C_ShootAimHUD);
			ShootAimHUD->AddToViewport();
		}
	}
}



void UBaseGameInstance::ShowCreatureHUD(bool bShow)
{
	if (IsValid(CreatureHUD))
	{
		CreatureHUD->RemoveFromViewport();
	}
	if (bShow)
	{

		if (IsValid(C_CreatureHUD))
		{
			CreatureHUD = CreateWidget<UUserWidget>(GetWorld(), C_CreatureHUD);
			CreatureHUD->AddToViewport();
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
	if (IsValid(DeadHUD))
	{
		DeadHUD->RemoveFromParent();
	}
	if (bShow)
	{
		if (IsValid(C_DeadHUD))
		{
			DeadHUD = CreateWidget<UUserWidget>(GetWorld(), C_DeadHUD);
			DeadHUD->AddToViewport();
		}
	}

}

void UBaseGameInstance::ShowKeyDeadHUD(bool bShow)
{
	UE_LOG(LogTemp, Warning, TEXT("UBaseGameInstance::ShowKeyDeadHUD"));
	if (IsValid(KeyDeadHUD))
	{
		KeyDeadHUD->RemoveFromParent();
	}
	if (bShow)
	{
		if (IsValid(C_KeyDeadHUD))
		{
			KeyDeadHUD = CreateWidget<UUserWidget>(GetWorld(), C_KeyDeadHUD);
			KeyDeadHUD->AddToViewport();
		}
	}

}
