// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Perception/PawnSensingComponent.h"
#include "GameFramework/Character.h"
#include "BaseCreature.h"
#include "Kismet/KismetMathLibrary.h"
#include "BaseDefinedStructs.h"
#include "BaseCharacter.h"
#include "BaseMonster.generated.h"



UCLASS()
class RPGTUTORIAL_API ABaseMonster : public ABaseCreature
{
GENERATED_BODY()

private:
	bool IsSprinting = false;


public:
	// Sets default values for this character's properties
	ABaseMonster();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
		class UPawnSensingComponent* PawnSensingComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float NearTime = 0;  // 被近身时间
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ActionInterval = 4;  // 每次行动间隔
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AngerSpawn = 0.15;  // 累计伤害百分比达到愤怒,影响着怪物使用特殊技能的频率
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AngerCount = 0;  // 累计伤害

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MonsterCore = 1;  // 怪物核心， 装载其上的额外技能的伤害基准

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseAttackPower;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EMonsterType MonsterType = EMonsterType::E_Normal;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Dead() override;

	// 定义SeePawn的回调
	UFUNCTION()
		void OnSeePawn(APawn* Pawn);

	UFUNCTION()
		void OnAttackChanged(bool Enable);
	UFUNCTION(BlueprintCallable)
		void OnAttackDamageEnableChanged(bool Enable);
	UFUNCTION()
		void OnAttackComboEnableChanged(bool Enable);
	//UFUNCTION(BlueprintCallable)
	//	void AttackDamageEnable(bool Enable);
	//UFUNCTION(BlueprintImplementableEvent)
	//	void BP_OnAttackDamageEnableChanged(bool Enable);

	virtual float AcceptDamage(float Damage, float Penetrate=0) override;
	
	UFUNCTION(BlueprintCallable)
	void ActionModes();

	UFUNCTION(BlueprintImplementableEvent)
	void BP_ActionModes();
	

	UFUNCTION(BlueprintCallable)
	void CalNearTime();

	UFUNCTION(BlueprintCallable)
	FRotator GetRotationToCharacter();

	UFUNCTION(BlueprintCallable)
	bool RotateToCharacter();

	UFUNCTION(BlueprintCallable)
	bool IsAnger();
	UFUNCTION(BlueprintCallable)
	void SetAngerCount(float Count);

	virtual void SetTarget(ABaseCreature* C) override;

};
