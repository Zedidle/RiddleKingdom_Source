// Fill out your copyright notice in the Description page of Project Settings.

#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GamePlayStatics.h"
#include "Blueprint/UserWidget.h"
#include "AIController.h"

#include "ActorComponents/BaseAttributeSet.h"
#include "Settings/BaseSaveGame.h"
#include "Settings/BaseGameInstance.h"
#include "Pawns/BaseCreature.h"
#include "Pawns/BaseMonster.h"


UBaseGameInstance::UBaseGameInstance()
{

}


void UBaseGameInstance::SetSetting(FSetting S)
{
	Setting = S;
}

void UBaseGameInstance::SaveSetting(UBaseSaveGame* SG)
{
	UGameplayStatics::SaveGameToSlot(SG, SettingFileName, 0);
}

UBaseSaveGame* UBaseGameInstance::LoadSetting()
{
	USaveGame* SavaGame = UGameplayStatics::LoadGameFromSlot(SettingFileName, 0);
	UBaseSaveGame* SG = Cast<UBaseSaveGame>(SavaGame);
	if (IsValid(SG))
	{
		Setting = SG->Setting;
	}
	return SG;
}








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

	RefreshAllHUD(true);
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
	if (IsValid(SG))
	{
		RefreshAllHUD(true);

		// 夺舍链
		CreatureUsed = SG->CreatureUsed;
		GameProgress = SG->GameProgress;

		TArray<AActor*> CreatureArray;
		UGameplayStatics::GetAllActorsOfClass(GWorld, ABaseCreature::StaticClass(), CreatureArray);

		// 复原所有在场 Creature
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
					C->BaseAttributeSet = D.BaseAttributeSet;
					C->bBeenControlled = D.bBeenControlled;
					C->Target = nullptr;
					C->ResetSave();

					// 控制中的角色继续控制
					if (D.bPlayerControlling)
					{
						PlayerController->UnPossess();
						PlayerController->Possess(C);
					}
					//// 控制过或死亡中的角色，除了原本就是盟友外，失去活动能力
					else if (D.bBeenControlled || D.BaseAttributeSet->CurHealth.GetCurrentValue() <= 0)
					{
						if (!C->IsPlayerControlled())
						{
							C->Dead(false);
						}
					}
					//// 没有控制过且没死过的
					else if (D.BaseAttributeSet->CurHealth.GetCurrentValue() > 0)
					{
						C->Revive();
					}
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
	if (IsValid(BossHealthHUD))
	{
		BossHealthHUD->RemoveFromViewport();
	}
	if (IsValid(M) && M->MonsterLevel == EMonsterLevel::E_Boss)
	{
		CurBoss = M;
		if (C_BossHealth)
		{
			BossHealthHUD = CreateWidget<UUserWidget>(GetWorld(), C_BossHealth);
			BossHealthHUD->AddToViewport();
		}
	}
}

ABaseMonster* UBaseGameInstance::GetCurBoss()
{
	return CurBoss;
}


void UBaseGameInstance::RefreshAllHUD(bool bShow)
{
	ShowCreatureHUD(bShow);
	ShowCreatureInfo(bShow);
	ShowEquipInfo(bShow);
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
	if (IsValid(CreatureInfoHUD))
	{
		CreatureInfoHUD->RemoveFromViewport();
	}
	if (bShow)
	{
		if (IsValid(C_CreatureInfoHUD))
		{
			CreatureInfoHUD = CreateWidget<UUserWidget>(GetWorld(), C_CreatureInfoHUD);
			CreatureInfoHUD->AddToViewport();
		}
	}
}


void UBaseGameInstance::ShowEquipInfo(bool bShow)
{
	UE_LOG(LogTemp, Warning, TEXT("UBaseGameInstance::ShowEquipInfo"));
	if (IsValid(EquipInfoHUD))
	{
		EquipInfoHUD->RemoveFromViewport();
	}
	if (bShow)
	{
		if (IsValid(C_EquipInfoHUD))
		{
			EquipInfoHUD = CreateWidget<UUserWidget>(GetWorld(), C_EquipInfoHUD);
			EquipInfoHUD->AddToViewport();
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
			DeadHUD->AddToViewport(1000);
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

void UBaseGameInstance::SetGmaeProgress(EGameProgress Progress)
{
	if (Progress > GameProgress)
	{
		GameProgress = Progress;
	}
}
