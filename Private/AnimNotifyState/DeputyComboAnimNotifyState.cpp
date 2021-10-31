// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState/DeputyComboAnimNotifyState.h"
#include "Pawns/BaseCharacter.h"
#include "Pawns/BaseMonster.h"
#include "Components/SkeletalMeshComponent.h"

void UDeputyComboAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	UE_LOG(LogTemp, Warning, TEXT("UDeputyComboAnimNotifyState::NotifyBegin"));
	if (MeshComp) {
		ABaseCharacter* Character = Cast<ABaseCharacter>(MeshComp->GetOwner());
		if (Character) {
			Character->OnDeputyComboEnableChanged(true);
			return;
		}
	}
}

void UDeputyComboAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	UE_LOG(LogTemp, Warning, TEXT("UDeputyComboAnimNotifyState::NotifyEnd"));
	if (MeshComp) {
		ABaseCharacter* Character = Cast<ABaseCharacter>(MeshComp->GetOwner());
		if (Character) {
			Character->OnDeputyComboEnableChanged(false);
			return;
		}
	}
}