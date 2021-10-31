// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState/AttackAnimNotifyState.h"
#include "Pawns/BaseCharacter.h"
#include "Pawns/BaseMonster.h"
#include "Components/SkeletalMeshComponent.h"



void UAttackAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	UE_LOG(LogTemp, Warning, TEXT("UAttackAnimNotifyState::NotifyBegin"));
	if (MeshComp) {
		ABaseCharacter* Character = Cast<ABaseCharacter>(MeshComp->GetOwner());
		if (Character) {
			Character->OnAttackChanged(true);
			return;
		}
		ABaseMonster* Monster = Cast<ABaseMonster>(MeshComp->GetOwner());
		if (Monster)
		{
			Monster->OnAttackChanged(true);
			return;
		}
	}
}

void UAttackAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	UE_LOG(LogTemp, Warning, TEXT("UAttackAnimNotifyState::NotifyEnd"));
	if (MeshComp) {
		ABaseCharacter* Character = Cast<ABaseCharacter>(MeshComp->GetOwner());
		if (Character) {
			Character->OnAttackChanged(false);
			return;
		}
		ABaseMonster* Monster = Cast<ABaseMonster>(MeshComp->GetOwner());
		if (Monster)
		{
			Monster->OnAttackChanged(false);
			return;
		}
	}
}
