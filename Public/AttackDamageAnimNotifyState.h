// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AttackDamageAnimNotifyState.generated.h"

/**
 * 
 */
UCLASS()
class RPGTUTORIAL_API UAttackDamageAnimNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()
private:
	void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;

	void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

};
