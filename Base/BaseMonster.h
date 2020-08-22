// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "BaseDefinedStructs.h"
#include "BaseCharacter.h"
#include "BaseMonster.generated.h"



typedef void(*Funcptr)();

//UFUNCTION();
//TFunction<void()> Func;

// 行动结构
USTRUCT(BlueprintType)
struct FMonsterAction
{
	GENERATED_BODY()

public:
	EDistance ActionDistance;  // 行动范围
	float ActionRate;  // 行动权重
	float ActionTime;  // 整体用时(包括行动间歇)
	Funcptr Func;  // 行动函数
};

// 行动列表


UCLASS()
class RPGTUTORIAL_API ABaseMonster : public ACharacter
{
	GENERATED_BODY()

private:


public:
	// Sets default values for this character's properties
	ABaseMonster();
	TSet<FMonsterAction> MonsterActionSet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float NearTime = 0;  // 被近身时间
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ActionInterval = 4;  // 每次行动间隔
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AngerSpawn = 0.15;  // 累计伤害百分比达到愤怒,影响着怪物使用特殊技能的频率
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AngerCount = 0;  // 累计伤害
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MonsterCore = 1;  // 怪物核心， 装载其上的额外技能的伤害基准
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Defense;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseAttackPower;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WalkSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EMonsterType MonsterType = EMonsterType::E_NORMAL;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class ABaseCharacter* CurCharacter = nullptr;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//class UPawnSensingComponent* Sensing;
	



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	FTimerHandle InvincibleTimer;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	bool IsDead();
	UFUNCTION(BlueprintCallable)
	void Dead();
	UFUNCTION(BlueprintImplementableEvent)
	void BP_Dead();



	UFUNCTION(BlueprintCallable)
	void AcceptDamage(float Damage, float Penetrate=0);
	UFUNCTION(BlueprintImplementableEvent)
	void BP_AcceptDamage();
	
	UFUNCTION(BlueprintCallable)
	void Welcome(ABaseCharacter* Character);

	UFUNCTION(BlueprintCallable)
	void ActionModes(float DeltaTime);

	UFUNCTION(BlueprintImplementableEvent)
	void BP_ActionModes();
	
	UFUNCTION(BlueprintCallable)
	EDistance GetDistanceTypeToCharacter();

	UFUNCTION(BlueprintCallable)
	void CalNearTime(float DeltaTime);

	UFUNCTION(BlueprintCallable)
	FRotator GetRotationToCharacter();

	UFUNCTION(BlueprintCallable)
	bool RotateToCharacter();

	UFUNCTION(BlueprintCallable)
	void SetMaxWalkSpeed(float Speed);

	//UFUNCTION(BlueprintCallable)
	//void AddMonsterAction(EDistance Distance, float ActionRate, float ActionTime, Funcptr Func);

	UFUNCTION(BlueprintCallable)
	void GetMonsterActionsByDistance(EDistance Distance);
	UFUNCTION(BlueprintCallable)
	void SetInvincible(float Time);
	
	UFUNCTION(BlueprintCallable)
	bool IsInvincible();

	UFUNCTION(BlueprintCallable)
	float GetHealthPercent();

	UFUNCTION(BlueprintCallable)
	bool IsAnger();
	UFUNCTION(BlueprintCallable)
	void SetAngerCount(float Count);

};