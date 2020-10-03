// Fill out your copyright notice in the Description page of Project Settings.


#include "DeputyDamageAnimNotifyState.h"
#include "RPGTutorial/Base/BaseCharacter.h"
#include "RPGTutorial/Base/BaseMonster.h"
#include "Components/SkeletalMeshComponent.h"

void UDeputyDamageAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	UE_LOG(LogTemp, Warning, TEXT("UDeputyDamageAnimNotifyState::NotifyBegin"));
	if (MeshComp) {
		ABaseCharacter* Character = Cast<ABaseCharacter>(MeshComp->GetOwner());
		if (Character) {
			Character->OnDeputyDamageEnableChanged(true);
			return;
		}
	}
}

void UDeputyDamageAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	UE_LOG(LogTemp, Warning, TEXT("UDeputyDamageAnimNotifyState::NotifyEnd"));
	if (MeshComp) {
		ABaseCharacter* Character = Cast<ABaseCharacter>(MeshComp->GetOwner());
		if (Character) {
			Character->OnDeputyDamageEnableChanged(false);
			return;
		}
	}
}