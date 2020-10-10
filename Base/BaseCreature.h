// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/Character.h"
#include "BaseDefinedStructs.h"
#include "BaseWeapon.h"
#include "BaseDeputy.h"
#include "ActionInterface.h"
#include "BaseCreature.generated.h"


UCLASS()
class RPGTUTORIAL_API ABaseCreature : public ACharacter, public IActionInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCreature();
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* SpringArm = nullptr;
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* Camera = nullptr;
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UCreatureTracer* CreatureTracer = nullptr;

	//FComboIndex ComboIndex;
	TMap<FString, int> ComboIndex;


public:
	// 是否作为AI
	UPROPERTY(EditAnywhere, BlueprintReadWrite) 
		bool IsAI = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float ActionInterval = 2;  // 每次行动间隔
	UFUNCTION(BlueprintCallable)
		void ActionModes();
	UFUNCTION(BlueprintImplementableEvent)
		void BP_ActionModes();

	// 当承受伤害大于生命百分比（StiffPercent）时，触发硬直动作
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float StiffPercent = 0.05; 


	UFUNCTION(BlueprintCallable)
		void SetMovement(float SpeedMulti = 2, float RotationRataZMulti = 2, EMovementMode Mode = EMovementMode::MOVE_None);

	UFUNCTION(BlueprintCallable)
		void SetDilation(float WorldScale, float SelfScale, float Time = 0.0f);
	UFUNCTION(BlueprintCallable)
		void InitDilation();


	// 所有操作定义
	UFUNCTION()
		virtual void JumpPress();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void BP_JumpPress();

	UFUNCTION()
		virtual void Sprinting();
	UFUNCTION()
		virtual void StopSprinting();

	UFUNCTION()
		virtual void N1Attack();
	UFUNCTION()
		virtual void N2Attack();

	UFUNCTION()
		virtual void UseDeputy();

	UFUNCTION()
		virtual void Lock();
	UFUNCTION()
		virtual void Dodge();
	UFUNCTION(BlueprintImplementableEvent)
		void BP_Dodge();
	UFUNCTION()
		virtual void Communicate();
	UFUNCTION(BlueprintImplementableEvent)
		void BP_Communicate();
	UFUNCTION()
		virtual void StopCommunicate();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void BP_StopCommunicate();
	UFUNCTION()
		virtual void MoveForward(float Amount);
	UFUNCTION(BlueprintImplementableEvent)
		void BP_MoveForward(float Amount);
	UFUNCTION()
		virtual void MoveRight(float Amount);
	UFUNCTION(BlueprintImplementableEvent)
		void BP_MoveRight(float Amount);
	UFUNCTION()
		virtual void MoveUp(float Amount);
	UFUNCTION(BlueprintImplementableEvent)
		void BP_MoveUp(float Amount);
	UFUNCTION()
		virtual void Turn(float Amount);
	UFUNCTION()
		virtual void LookUp(float Amount);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class ABaseCreature* Target = nullptr;

	class ABaseWeapon* TargetWeapon = nullptr;

	// 基础变量设置
	UPROPERTY(BlueprintReadOnly)
	float DeltaSeconds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsSprinting = false;

	bool CanAction = true; // 处于不同场合或动作判定是否可以攻击


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float CurHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float HealthRegen; // 每秒恢复生命值
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float CurStamina;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MaxStamina;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float StaminaRegen;	// 每秒恢复体力
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int Defense = 0;	// 防御力


	UFUNCTION()
		void Falling();

	// 计时器部分
	FTimerHandle DilationTimer;
	FTimerHandle InvincibleTimer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
		UDataTable* CreatureMontageDataTable;





protected:
	UFUNCTION()
		FString GetMovementModeString(FString Prefix = "");
		FString GetSubMovementModeString(FString Prefix = "");


public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float FallingTime = 0;  // 下落时间，用来处理跌落效果


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString CreatureID = "000";
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString CreatureName = "CreatureName";
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FCreatureType CreatureType = FCreatureType::E_None;


	// 运动部分
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseSpeed = 300;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseRotationRateZ = 120;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EFaction Faction = EFaction::E_None;

	// 作用仅仅是将角色和镜头的转向锁定到Target
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsLocking = false;

	// 战斗时是否给对方显示血条
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool ShowBlood = false;

	// 是否显示状态栏，影响复活后的角色界面，也是可以直接绑定。
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool ShowHUD = true;

	// 基础生命部分
	UFUNCTION(BlueprintCallable)
		bool IsDead();
	UFUNCTION(BlueprintCallable)
		virtual void Dead();
	UFUNCTION(BlueprintImplementableEvent)
		void BP_Dead();

	UFUNCTION(BlueprintCallable)
		virtual void Revive();
	UFUNCTION(BlueprintImplementableEvent)
		void BP_Revive();
	
	UFUNCTION()
		void Regen();
	

	UFUNCTION(BlueprintCallable)
	virtual	float AcceptDamage(float Damage, float Penetrate = 0);
	UFUNCTION(BlueprintImplementableEvent)
		void BP_AcceptDamage();




	// 运动状态判断
	UFUNCTION(BlueprintCallable)
		bool IsFalling();
	UFUNCTION(BlueprintCallable)
		bool IsFlying();
	UFUNCTION(BlueprintCallable)
		bool IsGround();
	UFUNCTION(BlueprintCallable)
		bool IsSwimming();
	UFUNCTION(BlueprintCallable)
		bool IsClimbing();
	UFUNCTION(BlueprintCallable)
		void SetClimbing(bool b);


	UFUNCTION(BlueprintCallable)
		void MontageStart(UAnimMontage* Montage);
	UFUNCTION(BlueprintImplementableEvent)
		void BP_MontageStart(UAnimMontage* Montage);
	UFUNCTION(BlueprintCallable)
		void MontageEnd(UAnimMontage* Montage, bool bInterrupted);
	UFUNCTION(BlueprintImplementableEvent)
		void BP_MontageEnd(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION(BlueprintCallable)
		float GetHealthPercent();
	UFUNCTION(BlueprintCallable)
		float GetStaminaPercent();
		
	// 设置/判断 无敌
	UFUNCTION(BlueprintCallable)
		void SetInvincible(float Time);
	UFUNCTION(BlueprintCallable)
		bool IsInvincible();

	UFUNCTION(BlueprintCallable)
		virtual void SetTarget(ABaseCreature* C);
	UFUNCTION(BlueprintCallable)
		ABaseCreature* GetTarget();




protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
		EDistance GetDistanceTypeToTarget();

	UFUNCTION(BlueprintCallable)
		bool PlayMontage(FString Rowname, FString SectionName = "", float PlayRate=1.0f);


};
