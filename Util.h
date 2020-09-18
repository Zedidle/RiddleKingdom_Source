// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TimerManager.h"
#include "Base/BaseDefinedStructs.h"
#include "Kismet/GameplayStatics.h"

namespace Util{
	FString GetWeaponTypeString(EWeaponType WeaponType, FString Prefix = "");
	FString GetDeputyTypeString(EDeputyType DeputyType, FString Prefix = "");
    
};







/**
 * 
 */
//class RPGTUTORIAL_API Util
//{
//
//
//private:
//	void DilationTimeout();
//
//
//public:
//	Util();
//	~Util();



//	FTimerHandle DilationTimer;
//	const UWorld* World;
//
//	UFUNCTION(BlueprintCallable, Category = "Utilities|Time", meta = (WorldContext = "WorldContextObject"))
//	static void SetGlobalTimeDilation(const UWorld* W, float Scale, float Time);
//	void SetDilationInTime(const UWorld* W, float Scale, float Time);
//
//
//
//};
