// Fill out your copyright notice in the Description page of Project Settings.


#include "DeputyAnimNotifyState.h"
#include "RPGTutorial/Base/BaseCharacter.h"
#include "RPGTutorial/Base/BaseMonster.h"
#include "Components/SkeletalMeshComponent.h"


void UDeputyAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	UE_LOG(LogTemp, Warning, TEXT("UDeputyAnimNotifyState::NotifyBegin"));
	if (MeshComp) {
		ABaseCharacter* Character = Cast<ABaseCharacter>(MeshComp->GetOwner());
		if (Character) {
			Character->OnDeputyUse(true);
			return;
		}
		ABaseMonster* Monster = Cast<ABaseMonster>(MeshComp->GetOwner());
		if (Monster)
		{
			return;
		}
	}
}

void UDeputyAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	UE_LOG(LogTemp, Warning, TEXT("UDeputyAnimNotifyState::NotifyEnd"));
	if (MeshComp) {
		ABaseCharacter* Character = Cast<ABaseCharacter>(MeshComp->GetOwner());
		if (Character) {
			Character->OnDeputyUse(false);
			return;
		}
		ABaseMonster* Monster = Cast<ABaseMonster>(MeshComp->GetOwner());
		if (Monster)
		{
			return;
		}
	}
}