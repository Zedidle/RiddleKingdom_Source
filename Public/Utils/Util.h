// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TimerManager.h"
#include "Utils/BaseDefinedStructs.h"
#include "Kismet/GameplayStatics.h"


namespace Util
{
	FString GetWeaponTypeString(EWeaponType WeaponType, FString Prefix = "");
	FString GetDeputyTypeString(EDeputyType DeputyType, FString Prefix = "");

	bool AbilityCanUseWeaponType(ECharacterAbility Ability, EWeaponType WeaponType);
	bool AbilityCanUseDeputyType(ECharacterAbility Ability, EDeputyType DeputyType);

	
	float CalDamageByDifficulty(float Damage, bool bPlayerControlled);



	UFUNCTION(Exec)
	void TestCmd();


};