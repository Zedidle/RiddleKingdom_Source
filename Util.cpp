// Fill out your copyright notice in the Description page of Project Settings.


#include "Util.h"
//#include "Kismet/KismetMathLibrary.h"
//using Math = UKismetMathLibrary;





FString Util::GetWeaponTypeString(EWeaponType WeaponType, FString Prefix)
{
	TMap<EWeaponType, FString> W;
	W.Add(EWeaponType::E_None, "");
	W.Add(EWeaponType::E_GreatSword, "GreatSword");
	W.Add(EWeaponType::E_Sword, "Sword");
	W.Add(EWeaponType::E_Bow, "Bow");
	W.Add(EWeaponType::E_MagicWand, "MagicWand");

	FString R = W[WeaponType];
	if (R != "")
	{
		R = Prefix + R;
	}
	return R;
}


FString Util::GetDeputyTypeString(EDeputyType DeputyType, FString Prefix)
{
	TMap<EDeputyType, FString> D;
	D.Add(EDeputyType::E_None, "");
	D.Add(EDeputyType::E_Shild, "Shield");
	D.Add(EDeputyType::E_Dagger, "Dagger");

	FString R = D[DeputyType];
	if (R != "")
	{
		R = Prefix + R;
	}

	return R;
}

bool Util::AbilityCanUseWeaponType(ECharacterAbility Ability, EWeaponType WeaponType)
{
	TMap<ECharacterAbility, TArray<EWeaponType>> Map_AbilityWeapons;

	TArray<EWeaponType> BowWeapons;
	BowWeapons.Add(EWeaponType::E_Bow);
	Map_AbilityWeapons.Add(ECharacterAbility::E_BOW, BowWeapons);


	TArray<EWeaponType> SwordWeapons;
	SwordWeapons.Add(EWeaponType::E_GreatSword);
	SwordWeapons.Add(EWeaponType::E_Sword);
	Map_AbilityWeapons.Add(ECharacterAbility::E_SWORD, SwordWeapons);


	TArray<EWeaponType> MagicWeapons;
	MagicWeapons.Add(EWeaponType::E_MagicWand);
	Map_AbilityWeapons.Add(ECharacterAbility::E_MAGIC, MagicWeapons);

	if (Map_AbilityWeapons.Contains(Ability))
	{
		return Map_AbilityWeapons[Ability].Contains(WeaponType);
	}
	else
	{
		return false;
	}
}


















//Util::Util()
//{
//}
//
//Util::~Util()
//{
//}
//void Util::SetGlobalTimeDilation(const UWorld* W, float Scale, float Time)
//{
//	if (W)
//	{
//		World = W;
//		UGameplayStatics::SetGlobalTimeDilation(World, Scale);
//		World->GetTimerManager().ClearTimer(DilationTimer);
//		World->GetTimerManager().SetTimer(DilationTimer, this, &Util::DilationTimeout, Time);
//	}
//}
//
//void Util::DilationTimeout()
//{
//	if (World)
//	{
//		UGameplayStatics::SetGlobalTimeDilation(World, 1);
//	}
//}
