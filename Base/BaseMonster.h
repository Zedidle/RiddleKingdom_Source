// 基础怪物，一般与BaseCharacter的阵营对立且拥有怒气值系统与更为充沛的体力值

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

	float AngerCount = 0;  // 累计伤害
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AngerSpawn = 0.15;  // 累计伤害百分比达到愤怒,影响着怪物使用特殊技能的频率

	float StiffCount = 0;  // 累计伤害
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StiffSpawn = 0.10;  // 累计伤害百分比达到硬直,影响着怪物受伤硬直的频率

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MonsterCore = 1;  // 怪物核心， 装载其上的额外技能的伤害基准

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseAttackPower;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EMonsterLevel MonsterLevel = EMonsterLevel::E_Normal;


	FTimerHandle StoicTimer;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void Tick_CalNearTime();










	virtual void Dead(bool bClearHealth = true) override;

	// 定义SeePawn的回调
	UFUNCTION()
		void OnSeePawn(APawn* Pawn);

	UFUNCTION()
		void OnAttackChanged(bool Enable);
	UFUNCTION()
		void OnAttackDamageEnableChanged(bool Enable);
	UFUNCTION(BlueprintImplementableEvent)
		void BP_OnAttackDamageEnableChanged(bool Enable);
	UFUNCTION()
		void OnAttackComboEnableChanged(bool Enable);


	virtual float AcceptDamage(float Damage, float Penetrate=0) override;
	
	UFUNCTION()
	void SetStoic(float Time);
	UFUNCTION()
	bool IsStoic();



	UFUNCTION(BlueprintCallable)
	FRotator GetRotationToCharacter();

	UFUNCTION(BlueprintCallable)
	bool RotateToCharacter();

	UFUNCTION(BlueprintCallable)
	bool IsAnger();
	UFUNCTION(BlueprintCallable)
	void SetAngerCount(float Count);


	UFUNCTION(BlueprintCallable)
		void Stiff();
	UFUNCTION(BlueprintCallable)
		void SetStiffCount(float Count);
	

	virtual void SetTarget(ABaseCreature* C) override;

};
