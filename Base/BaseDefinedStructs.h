// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedStruct.h"
#include "Engine/DataTable.h"
#include "Kismet/KismetSystemLibrary.h"
#include "BaseDefinedStructs.generated.h"






UENUM(BlueprintType)
enum class EFaction :uint8 // 技能阵营
{
	E_None  UMETA(DisplayName = "无"),	// Bow
	E_Character UMETA(DisplayName = "角色"),	// Bow
	E_Monster UMETA(DisplayName = "怪物")	// Magic
};

UENUM(BlueprintType, Blueprintable)
enum class EDistance :uint8 // 怪物行动范围
{
	E_PLUMB_NEAR UMETA(DisplayName = "垂直 近距离"),
	E_PLUMB_MID UMETA(DisplayName = "垂直 中距离"),
	E_PLUMB_FAR UMETA(DisplayName = "垂直 远距离"),
	E_PLUMB_SFAR UMETA(DisplayName = "垂直 超远距离"),


	E_FLAT_SNEAR UMETA(DisplayName = "水平 超近距离"),
	E_FLAT_NEAR UMETA(DisplayName = "水平 近距离"),
	E_FLAT_MID UMETA(DisplayName = "水平 中距离"),
	E_FLAT_FAR UMETA(DisplayName = "水平 远距离"),
	E_FLAT_SFAR UMETA(DisplayName = "水平 超远距离"),
};

UENUM(BlueprintType)
enum class ESkillType :uint8 // 技能类型
{
	E_Magic UMETA(DisplayName = "法术"),	
	E_Physics UMETA(DisplayName = "物理")
};


UENUM(BlueprintType)
enum class FCreatureType :uint8 // 怪物类型
{
	E_None  UMETA(DisplayName = "未知", ShortToolTip = "None"),
	E_Human  UMETA(DisplayName = "人族", ShortToolTip = "Human"),
	E_Troll  UMETA(DisplayName = "巨魔", ShortToolTip = "Troll"),
	E_Dragon UMETA(DisplayName = "龙族", ShortToolTip = "Dragon"),
	E_Goblin UMETA(DisplayName = "妖精", ShortToolTip = "Goblin"),
	E_Unreal UMETA(DisplayName = "虚空", ShortToolTip = "Unreal"),
};

UENUM(BlueprintType)
enum class EMonsterLevel :uint8 // 怪物阶层
{
	E_Boss UMETA(DisplayName = "BOSS"), // Sword
	E_Elite UMETA(DisplayName = "精英"),	// Bow
	E_Normal UMETA(DisplayName = "小怪")	// Magic
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


// 行动类型，每个角色不一定都要用到.. 可随时调整。 这里的内容并不重要，只是用作参考，后期再考虑用Enum写法代替FString.
UENUM(BlueprintType)
enum class EActionType :uint8 
{

	// 结构定义: E_操作_状态_子状态
	// 运动状态不写默认为地面

	// 基础通用
	E_Dead UMETA(DisplayName = "Dead"),
	E_Dead_Air UMETA(DisplayName = "Dead_Air"),
	E_Revive UMETA(DisplayName = "Revive"),
	E_TakeOff UMETA(DisplayName = "TakeOff"),
	E_Jump_Sprint UMETA(DisplayName = "Jump_Sprint"),
	E_Jump_Air UMETA(DisplayName = "Jump_Air"),
	E_Jump_Air_Sprint UMETA(DisplayName = "Jump_Air_Sprint"),
	E_Jump_FaceWall_Sprint UMETA(DisplayName = "Jump_FaceWall_Sprint"),
	E_Jump_FaceWall UMETA(DisplayName = "Jump_FaceWall"),
	E_Ledge_Climbing UMETA(DisplayName = "Ledge_Climbing"),
	E_Stiff UMETA(DisplayName = "Stiff"),
	E_StopTurn180 UMETA(DisplayName = "StopTurn180"),
	E_Dodge UMETA(DisplayName = "Dodge"),
	E_HighFall UMETA(DisplayName = "HighFall"),
	E_Repel UMETA(DisplayName = "Repel"),
	




	// 各类副手
	E_UseDeputy UMETA(DisplayName = "UseDeputy"),
	E_UseDeputy_Shield UMETA(DisplayName = "UseDeputy_Shield"),
	E_UseDeputy_Dagger UMETA(DisplayName = "UseDeputy_Dagger"),




	// 结构定义: E_操作_运动状态_运动子状态_武器类型
	// 运动状态不写默认为地面, 运动子状态不写默认为无，武器类型不写默认为无
	


	// 无主手
	E_N1Attack UMETA(DisplayName = "N1Attack"),
	E_N2Attack UMETA(DisplayName = "N2Attack"),
	E_N1Attack_Sprint UMETA(DisplayName = "N1Attack_Sprint"),
	E_N2Attack_Sprint UMETA(DisplayName = "N2Attack_Sprint"),
	E_N1Attack_Air UMETA(DisplayName = "N1Attack_Air"),
	E_N2Attack_Air UMETA(DisplayName = "N2Attack_Air"),
	E_N1Attack_Air_Sprint UMETA(DisplayName = "N1Attack_Air_Sprint"),
	E_N2Attack_Air_Sprint UMETA(DisplayName = "N2Attack_Air_Sprint"),




	// 主手 - GreatSword
	E_N1Attack_GreatSword UMETA(DisplayName = "N1Attack_GreatSword"),
	E_N2Attack_GreatSword UMETA(DisplayName = "N2Attack_GreatSword"),
	E_N1Attack_Sprint_GreatSword UMETA(DisplayName = "N1Attack_Sprint_GreatSword"),
	E_N2Attack_Sprint_GreatSword UMETA(DisplayName = "N2Attack_Sprint_GreatSword"),
	E_N1Attack_Air_GreatSword UMETA(DisplayName = "N1Attack_Air_GreatSword"),
	E_N2Attack_Air_GreatSword UMETA(DisplayName = "N2Attack_Air_GreatSword"),
	E_N1Attack_Air_Sprint_GreatSword UMETA(DisplayName = "N1Attack_Air_Sprint_GreatSword"),
	E_N2Attack_Air_Sprint_GreatSword UMETA(DisplayName = "N2Attack_Air_Sprint_GreatSword"),




	// 主手 - Bow
	E_N1Attack_Bow UMETA(DisplayName = "N1Attack_Bow"),
	E_N2Attack_Bow UMETA(DisplayName = "N2Attack_Bow"),
	E_N1Attack_Sprint_Bow UMETA(DisplayName = "N1Attack_Sprint_Bow"),
	E_N2Attack_Sprint_Bow UMETA(DisplayName = "N2Attack_Sprint_Bow"),
	E_N1Attack_Air_Bow UMETA(DisplayName = "N1Attack_Air_Bow"),
	E_N2Attack_Air_Bow UMETA(DisplayName = "N2Attack_Air_Bow"),
	E_N1Attack_Air_Sprint_Bow UMETA(DisplayName = "N1Attack_Air_Sprint_Bow"),
	E_N2Attack_Air_Sprint_Bow UMETA(DisplayName = "N2Attack_Air_Sprint_Bow"),
};



USTRUCT(BlueprintType)
struct FCreatureSaveData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector WorldLocation = FVector(0, 0, 0); // 当时的世界坐标
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
	float PlayRate = 1.0f;  // 武器的使用效率，模拟轻重、攻速加成
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
struct FCreatureMontage: public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAnimMontage* Montage;
};








/*
	=========================  装备区 =========================
*/


UENUM(BlueprintType)
enum class EWeaponType :uint8 // 主武器类型
{
	E_None UMETA(DisplayName = "None"),
	E_GreatSword UMETA(DisplayName = "GreatSword"),
	E_Sword UMETA(DisplayName = "Sword"),
	E_Bow UMETA(DisplayName = "Bow"),
	E_MagicWand UMETA(DisplayName = "MagicWand")
};

UENUM(BlueprintType)
enum class EDeputyType :uint8  // 副手类型
{
	E_None UMETA(DisplayName = "无"),
	E_Shild UMETA(DisplayName = "Shild"), // 盾牌基本效果：使用者无敌一些时间，额外效果：每个盾牌都不同；
	E_Dagger UMETA(DisplayName = "Dagger"), // 破防/涂毒
};

UENUM(BlueprintType)
enum class EArmorType :uint8  // 防具类型
{
	E_None UMETA(DisplayName = "无"),
	E_Helmet UMETA(DisplayName = "头盔"),
	E_Barcer UMETA(DisplayName = "护臂"),
	E_Glove UMETA(DisplayName = "手套"),
	E_Ring UMETA(DisplayName = "戒指"),
	E_Jacket UMETA(DisplayName = "上衣"),
	E_Cape UMETA(DisplayName = "披风"),
	E_Trousers UMETA(DisplayName = "裤子"),
	E_Shoes UMETA(DisplayName = "鞋子"),
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

USTRUCT(BlueprintType)
struct FDeputy : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString DeputyName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EDeputyType DeputyType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString DeputyDescription;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString DeputyActorPath;
};

USTRUCT(BlueprintType)
struct FArmor : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString ArmorName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EArmorType ArmorType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString ArmorDescription;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString ArmorActorPath;
};














USTRUCT(BlueprintType)
struct FCreature : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString CreatureID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString CreatureName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FCreatureType CreatureType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString CreatureDescription;
};





USTRUCT(BlueprintType)
struct FDialogue : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FString> Contents;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString TaskID;
};

USTRUCT(BlueprintType)
struct FTask : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString Description;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString TaskName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float RewardExp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString TaskID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSet<FString> RewardItemIDs;
};



USTRUCT(BlueprintType)
struct FTransportPoint : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString ID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString Description;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector WorldLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString TargetID;

};
