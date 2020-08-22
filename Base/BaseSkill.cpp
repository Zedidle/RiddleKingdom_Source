// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseSkill.h"
#include "BaseDefinedStructs.h"
#include "BaseMonster.h"
#include "BaseCharacter.h"




// Sets default values
ABaseSkill::ABaseSkill()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 不可行
	//if (GetOwner())
	//{
	//	if (Cast<ABaseCharacter>(GetOwner()))
	//	{
	//		SkillSide = ESkillSide::E_Character;
	//		UE_LOG(LogTemp, Warning, TEXT("ABaseSkill::E_Character"));
	//	}
	//	else if (Cast<ABaseMonster>(GetOwner()))
	//	{
	//		SkillSide = ESkillSide::E_Monster;
	//		UE_LOG(LogTemp, Warning, TEXT("ABaseSkill::E_Monster"));
	//	}
	//}
}

ABaseSkill::ABaseSkill(ESkillSide Side)
{
	PrimaryActorTick.bCanEverTick = true;
	SkillSide = Side;
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

void ABaseSkill::SetSkillInfo(ESkillSide Side, ESkillType Type, AActor* User)
{
	UE_LOG(LogTemp, Warning, TEXT("DamageToMonster SetSkillInfo"));
	SkillSide = Side;
	SkillType = Type;
	SkillUser = User;
}

bool ABaseSkill::MakeDamageToActor(AActor* Actor, float ExtraDamage)
{
	UE_LOG(LogTemp, Warning, TEXT("MakeDamageToActor"));
	if (!IsValid(SkillUser))
	{
		UE_LOG(LogTemp, Error, TEXT("技能必须设置使用者"));
		return false;
	}

	switch (SkillSide)
	{
		case ESkillSide::E_Character:
		{
			ABaseMonster* Monster = Cast<ABaseMonster>(Actor);
			ABaseCharacter* Character = Cast<ABaseCharacter>(SkillUser);
			if (Monster)
			{
				float AdditionDamage = Character->Weapon->GetDamage();
				float Damage = BaseDamage + ExtraDamage + AdditionDamage;
				Monster->AcceptDamage(Damage);
				//UE_LOG(LogTemp, Warning, TEXT("DamageToMonster AdditionDamage ： %d"), AdditionDamage);
				return true;
			}
		}
		case ESkillSide::E_Monster:
		{
			ABaseCharacter* Character = Cast<ABaseCharacter>(Actor);
			ABaseMonster* Monster = Cast<ABaseMonster>(SkillUser);
			if (Character)
			{
				float AdditionDamage = Monster->MonsterCore * SkillCore;
				float Damage = BaseDamage + ExtraDamage + AdditionDamage;
				Character->AcceptDamage(Damage);
				UE_LOG(LogTemp, Warning, TEXT("DamageToCharacter Damage, AdditionDamage  ： %f"), AdditionDamage);
				return true;

			}
		}
	}
	return false;
}


