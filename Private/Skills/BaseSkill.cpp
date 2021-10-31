

#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayAbilitySpec.h"
#include "AbilitySystemComponent.h"

#include "Skills/BaseSkill.h"
#include "Pawns/BaseMonster.h"
#include "Pawns/BaseCharacter.h"
#include "Utils/BaseDefinedStructs.h"


// Sets default values
ABaseSkill::ABaseSkill()
{
	PrimaryActorTick.bCanEverTick = true;
	AbilitySystemComp = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComp");

}

// Called when the game starts or when spawned
void ABaseSkill::BeginPlay()
{
	Super::BeginPlay();


	ABaseCreature* SkillUser = GetSkillUser();
	if (IsValid(SkillUser))
	{
		SkillUser->AddSkillOnUsing(this);
		//EventTag = SkillUser->ActiveEventTag;
		UE_LOG(LogTemp, Error, TEXT("ABaseSkill::BeginPlay EventTag: %s"), *EventTag.ToString());
	}
}

// Called every frame
void ABaseSkill::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

EFaction ABaseSkill::GetFaction()
{
	ABaseCreature* SkillUser = GetSkillUser();
	if(IsValid(SkillUser))
	{
		return SkillUser->Faction;
	}
	return EFaction::E_None;
}

ABaseCreature* ABaseSkill::GetSkillUser()
{
	if (IsValid(GetInstigator()))
	{
		ABaseCreature* Creature = Cast<ABaseCreature>(GetInstigator());
		if (IsValid(Creature))
		{
			return Creature;
		}
	}
	return nullptr;
}

void ABaseSkill::AddChildSkill(ABaseSkill* Skill)
{
	if (IsValid(Skill))
	{
		ChildSkills.AddUnique(Skill);
	}
}

void ABaseSkill::K2_DestroyActor()
{
	Super::K2_DestroyActor();
	for (auto& Skill : ChildSkills)
	{
		if (IsValid(Skill))
		{
			Skill->K2_DestroyActor();
		}
	}
}

void ABaseSkill::SendGameplayEventToActor()
{
}

bool ABaseSkill::MakeDamage(ABaseCreature* _Target)
{
	if (!IsValid(_Target))
	{
		_Target = Target;
	}
	ABaseCreature* Creature = GetSkillUser();
	UE_LOG(LogTemp, Error, TEXT("Creature->MakeDamage："));

	if (!IsValid(_Target) || _Target->IsDead() || _Target == Creature || 
	_Target->Faction == GetFaction() || _Target->IsInvincible()) return false;

	// GAS 
	Payload.Instigator = GetInstigator();
	Payload.Target = _Target;
	UE_LOG(LogTemp, Error, TEXT("Creature->MakeDamage EventTag：%s"), *EventTag.GetTagName().ToString() );

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, EventTag, Payload);
	return true;
}


UAbilitySystemComponent* ABaseSkill::GetAbilitySystemComponent() const
{
	return AbilitySystemComp;
}


void ABaseSkill::GetAbility(TSubclassOf<UGameplayAbility> AbilityType)
{
	if (nullptr != AbilityType && nullptr != AbilitySystemComp)
	{
		AbilitySystemComp->GiveAbility(FGameplayAbilitySpec(AbilityType));
		AbilitySystemComp->InitAbilityActorInfo(this, this);
	}
}