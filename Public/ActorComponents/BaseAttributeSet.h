// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "BaseAttributeSet.generated.h"


// 在使用GAS后，直接迁移
//USTRUCT(BlueprintType)
//struct FCharacterAttributes
//{
//	GENERATED_BODY()
//
//		// 属性计算
//public:
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//		float Power = 1;  // 力量
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//		float Sprit = 1;  // 精神
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//		float Body = 1;   // 体力
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//		float Agile = 1;  // 敏捷
//};

class UAttributeSet;

/**
 * 
 */
UCLASS()
class RPGTUTORIAL_API UBaseAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	public:
		UBaseAttributeSet();


	//// 基础生命体系
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FGameplayAttributeData CurHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FGameplayAttributeData MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FGameplayAttributeData HealthRegen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FGameplayAttributeData CurStamina;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FGameplayAttributeData MaxStamina;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FGameplayAttributeData StaminaRegen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FGameplayAttributeData CurMana;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FGameplayAttributeData MaxMana;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FGameplayAttributeData ManaRegen;


	//// 基础能力值
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FGameplayAttributeData Power;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FGameplayAttributeData Sprit;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FGameplayAttributeData Body;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FGameplayAttributeData Agile;


	//// 基础战力
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FGameplayAttributeData Aggressivity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FGameplayAttributeData Defense;
};
