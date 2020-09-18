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
