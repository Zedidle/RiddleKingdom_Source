// Fill out your copyright notice in the Description page of Project Settings.


#include "DeputyComboAnimNotifyState.h"
#include "RPGTutorial/Base/BaseCharacter.h"
#include "RPGTutorial/Base/BaseMonster.h"
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
		ABaseMonster* Monster = Cast<ABaseMonster>(MeshComp->GetOwner());
		if (Monster)
		{
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
		ABaseMonster* Monster = Cast<ABaseMonster>(MeshComp->GetOwner());
		if (Monster)
		{
			return;
		}
	}
}