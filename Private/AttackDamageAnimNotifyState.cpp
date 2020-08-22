// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackDamageAnimNotifyState.h"
#include "RPGTutorial/Base/BaseCharacter.h"
#include "Components/SkeletalMeshComponent.h"



void UAttackDamageAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	//UE_LOG(LogTemp, Warning, TEXT("UAttackDamageAnimNotifyState::NotifyBegin"));
	if (MeshComp) {
		ABaseCharacter* Owner = Cast<ABaseCharacter>(MeshComp->GetOwner());
		if (Owner) {
			Owner->OnAttackDamageEnableChanged(true);
		}
	}
}

void UAttackDamageAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	//UE_LOG(LogTemp, Warning, TEXT("UAttackDamageAnimNotifyState::NotifyEnd"));
	if (MeshComp) {
		ABaseCharacter* Owner = Cast<ABaseCharacter>(MeshComp->GetOwner());
		if (Owner) {
			Owner->OnAttackDamageEnableChanged(false);

		}
	}
}