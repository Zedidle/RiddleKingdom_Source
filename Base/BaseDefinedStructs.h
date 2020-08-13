// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedStruct.h"
#include "Engine\DataTable.h"

#include "BaseDefinedStructs.generated.h"


USTRUCT(BlueprintType)
struct FBaseGroundSound : public FTableRowBase // 地面类型 以及 踩踏声音路径
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString GroundDescription;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString SoundPath;
};


USTRUCT(BlueprintType)
struct FWeapon : public FTableRowBase 
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString WeaponName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString WeaponDescription;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString WeaponActorPath;
};


UENUM(BlueprintType)
enum class ECharacterAbility :uint8 // 角色职业类型
{
	E_SWORD UMETA(DisplayName = "剑士"),
	E_BOW UMETA(DisplayName = "弓手"),
	E_MAGIC UMETA(DisplayName = "法师")
};

UENUM(BlueprintType)
enum class EWeaponType :uint8 // 武器类型
{
	E_NONE UMETA(DisplayName = "无"),
	E_SWORD UMETA(DisplayName = "大剑"),
	E_BOW UMETA(DisplayName = "弓箭"),
	E_MAGIC UMETA(DisplayName = "法杖")
};

