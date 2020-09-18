// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseSkill.h"
#include "BaseMonster.h"
#include "BaseCharacter.h"
#include "BaseDefinedStructs.h"




// Sets default values
ABaseSkill::ABaseSkill()
{
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABaseSkill::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseSkill::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseSkill::SetSkillInfo(ESkillType Type, ABaseCreature* User)
{
	UE_LOG(LogTemp, Warning, TEXT("ABaseSkill::SetSkillInfo"));
	SkillType = Type;
	SkillUser = User;
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

bool ABaseSkill::MakeDamage(ABaseCreature* Target, float ExtraDamage)
{
	//UE_LOG(LogTemp, Warning, TEXT("ABaseSkill::MakeDamage"));

	if(!IsValid(Target) || Target->IsDead()) return false;

	//UE_LOG(LogTemp, Warning, TEXT("ABaseSkill::MakeDamage 000"));

	if (!IsValid(SkillUser))
	{
		UE_LOG(LogTemp, Error, TEXT("技能必须设置使用者"));
		return false;
	}
	//UE_LOG(LogTemp, Warning, TEXT("ABaseSkill::MakeDamage 111"));

	if (Target->Faction != GetFaction())
	{
		float AdditionDamage = 0;
		//UE_LOG(LogTemp, Warning, TEXT("ABaseSkill::MakeDamage 222"));

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
		Target->AcceptDamage(Damage);
		return true;
	}
	return false;
}


