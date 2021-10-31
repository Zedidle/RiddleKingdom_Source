// Fill out your copyright notice in the Description page of Project Settings.

#include "ActorComponents/BaseGameplayAbility.h"
#include "Pawns/BaseCreature.h"



void UBaseGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	ABaseCreature* Creature = Cast<ABaseCreature>(GetActorInfo().OwnerActor);
	if (IsValid(Creature))
	{
		Creature->ActiveEventTag = EventTag;
	}
}

void UBaseGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	ABaseCreature* Creature = Cast<ABaseCreature>(GetActorInfo().OwnerActor);
	if (IsValid(Creature))
	{
		Creature->ActiveEventTag = FGameplayTag::EmptyTag;
	}
}


