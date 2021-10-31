// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState/AttackComboAnimNotifyState.h"
#include "Pawns/BaseCharacter.h"
#include "Pawns/BaseMonster.h"
#include "Components/SkeletalMeshComponent.h"



void UAttackComboAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	UE_LOG(LogTemp, Warning, TEXT("UAttackComboAnimNotifyState::NotifyBegin"));
	if (MeshComp) {
		ABaseCharacter* Owner = Cast<ABaseCharacter>(MeshComp->GetOwner());
		if (Owner) {
			Owner->OnAttackComboEnableChanged(true);
		}
		ABaseMonster* Monster = Cast<ABaseMonster>(MeshComp->GetOwner());
		if (Monster)
		{
			Monster->OnAttackComboEnableChanged(true);
			return;
		}
	}
}

void UAttackComboAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	UE_LOG(LogTemp, Warning, TEXT("UAttackComboAnimNotifyState::NotifyEnd"));
	if (MeshComp) {
		ABaseCharacter* Owner = Cast<ABaseCharacter>(MeshComp->GetOwner());
		if (Owner) {
			Owner->OnAttackComboEnableChanged(false);
		}
		ABaseMonster* Monster = Cast<ABaseMonster>(MeshComp->GetOwner());
		if (Monster)
		{
			Monster->OnAttackComboEnableChanged(false);
			return;
		}
	}
}