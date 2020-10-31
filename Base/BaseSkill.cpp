// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseSkill.h"
#include "BaseMonster.h"
#include "BaseCharacter.h"
#include "BaseDefinedStructs.h"




// Sets default values
ABaseSkill::ABaseSkill()
{
	PrimaryActorTick.bCanEverTick = true;
	UE_LOG(LogTemp, Warning, TEXT("ABaseSkill::ABaseSkill"));

}

// Called when the game starts or when spawned
void ABaseSkill::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("ABaseSkill::BeginPlay"));
	if (IsValid(SkillUser))
	{
		UE_LOG(LogTemp, Warning, TEXT("ABaseSkill::BeginPlay SkillUser IsValid"));
		SkillUser->AddSkillOnUsing(this);
	}
	
}

// Called every frame
void ABaseSkill::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseSkill::SetSkillInfo(ESkillType Type, ABaseCreature* User)
{
	//UE_LOG(LogTemp, Warning, TEXT("ABaseSkill::SetSkillInfo"));
	SkillType = Type;
	SkillUser = User;
	User->AddSkillOnUsing(this);

}

EFaction ABaseSkill::GetFaction()
{
	if (IsValid(SkillUser))
	{
		return SkillUser->Faction;
	}
	else
	{
		return EFaction::E_None;
	}
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

bool ABaseSkill::MakeDamage(ABaseCreature* _Target, float ExtraDamage)
{
	if (!IsValid(_Target))
	{
		_Target = Target;
	}

	if(!IsValid(_Target) || _Target->IsDead()) return false;

	if (!IsValid(SkillUser))
	{
		UE_LOG(LogTemp, Error, TEXT("技能必须设置使用者"));
		return false;
	}

	if (_Target->Faction != GetFaction())
	{
		float AdditionDamage = 0;
		if (Cast<ABaseCharacter>(SkillUser))
		{
			ABaseWeapon* W = Cast<ABaseCharacter>(SkillUser)->Weapon;
			if (IsValid(W))
			{
				AdditionDamage = W->GetDamage();
			}
		}
		if (Cast<ABaseMonster>(SkillUser))
		{
			AdditionDamage = Cast<ABaseMonster>(SkillUser)->MonsterCore * SkillCore;
		}

		float Damage = BaseDamage + ExtraDamage + AdditionDamage;
		float TrueDamage = _Target->AcceptDamage(Damage);
		_Target->SetTarget(SkillUser);
		_Target->bCombating = true;
		return TrueDamage > 0;
	}
	return false;
}


