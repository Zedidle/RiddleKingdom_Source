// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState/AttackDamageAnimNotifyState.h"
#include "Components/SkeletalMeshComponent.h"
#include "Pawns/BaseCharacter.h"
#include "Pawns/BaseMonster.h"



void UAttackDamageAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	//UE_LOG(LogTemp, Warning, TEXT("UAttackDamageAnimNotifyState::NotifyBegin"));
	if (MeshComp) {
		ABaseCharacter* Character = Cast<ABaseCharacter>(MeshComp->GetOwner());
		if (Character) {
			Character->OnAttackDamageEnableChanged(true);
			return ;
		}
		ABaseMonster* Monster = Cast<ABaseMonster>(MeshComp->GetOwner());
		if (Monster)
		{
			Monster->OnAttackDamageEnableChanged(true);
			return;
		}
	}
}

void UAttackDamageAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	//UE_LOG(LogTemp, Warning, TEXT("UAttackDamageAnimNotifyState::NotifyEnd"));
	if (MeshComp) {
		ABaseCharacter* Character = Cast<ABaseCharacter>(MeshComp->GetOwner());
		if (Character) {
			Character->OnAttackDamageEnableChanged(false);
			return;
		}
		ABaseMonster* Monster = Cast<ABaseMonster>(MeshComp->GetOwner());
		if (Monster)
		{
			Monster->OnAttackDamageEnableChanged(false);
			return;
		}
	}
}