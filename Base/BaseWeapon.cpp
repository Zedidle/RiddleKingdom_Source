// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "BaseMonster.h"
#include "Kismet/KismetMathLibrary.h"




// Sets default values
ABaseWeapon::ABaseWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMesh");
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//Box = CreateDefaultSubobject<UBoxComponent>("Box");
	//Box->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

// Called when the game starts or when spawned
void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseWeapon::SetCurAbility(const FCharacterAttributes Attributes)
{

	switch (WeaponType)
	{
		case EWeaponType::E_BOW:
		{
			Ability.CritAddition = 0.5 + Attributes.Agile * WeaponCore;
			Ability.CritRate = 0.01 + Attributes.Agile * WeaponCore / 2;
			Ability.Damage = (7 * Attributes.Agile + 3 * Attributes.Power) * WeaponCore;
		}
		case EWeaponType::E_GREATSWORD:
		{
			Ability.CritAddition = 0.5 + Attributes.Agile * WeaponCore * 2;
			Ability.CritRate = 0.01 + Attributes.Agile * WeaponCore / 3;
			Ability.Damage = (7 * Attributes.Power + 3 * Attributes.Body) * WeaponCore;
		}
		case EWeaponType::E_MAGIC:
		{
			Ability.CritAddition = 0.5 + Attributes.Sprit * WeaponCore;
			Ability.CritRate = 0.01 + Attributes.Sprit * WeaponCore / 10;
			Ability.Damage = (10 * Attributes.Power) * WeaponCore;
		}
		case EWeaponType::E_SWORD:
		{
			Ability.CritAddition = 0.5 + Attributes.Agile * WeaponCore * 1.5;
			Ability.CritRate = 0.01 + Attributes.Agile * WeaponCore / 2.5;
			Ability.Damage = (3 * Attributes.Agile + 5 * Attributes.Power + 2 * Attributes.Body) * WeaponCore;
		}
	}
}

float ABaseWeapon::GetDamage()
{
	float Rand = UKismetMathLibrary::RandomFloat();
	if (Rand < Ability.CritRate)
	{
		return Ability.Damage * (1 + Ability.CritAddition);
	}
	else
	{
		return Ability.Damage;
	}
}

EWeaponType ABaseWeapon::GetWeaponType()
{
	return WeaponType;
}


bool ABaseWeapon::MakeDamageToActor(AActor* Actor, float ExtraDamage)
{
	UE_LOG(LogTemp, Warning, TEXT("MakeDamageToActor"));
	ABaseMonster* Monster = Cast<ABaseMonster>(Actor);
	if (Monster)
	{
		float Damage = GetDamage() + ExtraDamage;
		Monster->AcceptDamage(Damage);
		//UE_LOG(LogTemp, Warning, TEXT("DamageToMonster AdditionDamage ： %d"), AdditionDamage);
		return true;
	}
	return false;
}