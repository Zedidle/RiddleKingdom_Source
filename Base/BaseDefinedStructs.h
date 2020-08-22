// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedStruct.h"
#include "Engine/DataTable.h"
#include "Kismet/KismetSystemLibrary.h"

#include "BaseDefinedStructs.generated.h"


UENUM(BlueprintType)
enum class ESkillSide :uint8 // 技能阵营
{
	E_Character UMETA(DisplayName = "角色"),	// Bow
	E_Monster UMETA(DisplayName = "怪物")	// Magic
};

UENUM(BlueprintType)
enum class EDistance :uint8 // 怪物行动范围
{
	E_PLUMB_NEAR UMETA(DisplayName = "垂直 近距离 200-500"),
	E_PLUMB_MID UMETA(DisplayName = "垂直 中距离 500-1000"),
	E_PLUMB_FAR UMETA(DisplayName = "垂直 远距离 1000-1500"),
	E_PLUMB_SFAR UMETA(DisplayName = "垂直 超远距离 >1500"),

	E_FLAT_NEAR UMETA(DisplayName = "水平 近距离 0-700"),
	E_FLAT_MID UMETA(DisplayName = "水平 中距离 700-1500"),
	E_FLAT_FAR UMETA(DisplayName = "水平 远距离 1500-2400"),
	E_FLAT_SFAR UMETA(DisplayName = "水平 超远距离 >2400"),
};



UENUM(BlueprintType)
enum class ESkillType :uint8 // 技能类型
{
	E_MAGIC UMETA(DisplayName = "法术"),	
	E_PHYSICS UMETA(DisplayName = "物理")
};

UENUM(BlueprintType)
enum class EMonsterType :uint8 // 怪物类型
{
	E_BOSS UMETA(DisplayName = "BOSS"), // Sword
	E_Elite UMETA(DisplayName = "精英"),	// Bow
	E_NORMAL UMETA(DisplayName = "小怪")	// Magic
};


UENUM(BlueprintType)
enum class EWeaponType :uint8 // 武器类型
{
	E_NONE UMETA(DisplayName = "无"),
	E_GREATSWORD UMETA(DisplayName = "巨剑"),
	E_SWORD UMETA(DisplayName = "剑"),
	E_BOW UMETA(DisplayName = "弓箭"),
	E_MAGIC UMETA(DisplayName = "法杖")
};





UENUM(BlueprintType)
enum class ECharacterAbility :uint8 // 角色职业类型
{
	E_SWORD UMETA(DisplayName = "剑士"), // Sword
	E_BOW UMETA(DisplayName = "弓手"),	// Bow
	E_MAGIC UMETA(DisplayName = "法师")	// Magic
};



UENUM(BlueprintType)
enum class EGroundType :uint8 // 地面类型
{
	E_NONE UMETA(DisplayName = "无声"),
	E_GRASS UMETA(DisplayName = "草地"),
	E_ROCK UMETA(DisplayName = "岩地"),
	E_SNOW UMETA(DisplayName = "雪地"),
	E_SAND UMETA(DisplayName = "沙地")
};

USTRUCT(BlueprintType)
struct FCharacterAttributes
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Power = 1;  // 力量
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Sprit = 1;  // 精神
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Body = 1;   // 体力
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Agile = 1;  // 敏捷
};

USTRUCT(BlueprintType)
struct FWeaponAbility
{
	GENERATED_BODY()

public:
	float Damage = 1;  // 伤害
	float CritRate = 0.01;  // 暴击概率
	float CritAddition = 0.5;  // 暴击加成
};



USTRUCT(BlueprintType)
struct FBaseGroundSound : public FTableRowBase // 地面类型 以及 踩踏声音路径
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString SoundPath;
};

USTRUCT(BlueprintType)
struct FCharacterMontage: public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAnimMontage* Montage;
};


USTRUCT(BlueprintType)
struct FWeapon : public FTableRowBase 
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString WeaponName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EWeaponType WeaponType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString WeaponDescription;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString WeaponActorPath;
};




