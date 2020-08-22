// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackAnimNotifyState.h"
#include "RPGTutorial/Base/BaseCharacter.h"
#include "Components/SkeletalMeshComponent.h"



void UAttackAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	UE_LOG(LogTemp, Warning, TEXT("UAttackAnimNotifyState::NotifyBegin"));
	if (MeshComp) {
		ABaseCharacter* Owner = Cast<ABaseCharacter>(MeshComp->GetOwner());
		if (Owner) {
			Owner->OnAttackChanged(true);
		}
	}
}

void UAttackAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	UE_LOG(LogTemp, Warning, TEXT("UAttackAnimNotifyState::NotifyEnd"));
	if (MeshComp) {
		ABaseCharacter* Owner = Cast<ABaseCharacter>(MeshComp->GetOwner());
		if (Owner) {
			Owner->OnAttackChanged(false);

		}
	}
}
