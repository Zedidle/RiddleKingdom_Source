// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseDefinedStructs.h"
#include "BaseWeapon.h"
#include "BaseCharacter.generated.h"



UENUM(BlueprintType)
enum class EActionType :uint8 // 怪物行动范围
{
	E_NormalAttack UMETA(DisplayName = "普通攻击Combo NormalAttack"),
	E_N2Attack UMETA(DisplayName = "N2攻击Combo N2Attack"),
	E_Jump UMETA(DisplayName = "跳跃Combo Jump"),
};



UCLASS()
class RPGTUTORIAL_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UCameraComponent* Camera = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USpringArmComponent* SpringArm = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<UUserWidget> BaseHUD = nullptr;

	UPROPERTY()
		class ABaseWeapon* Weapon = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
		UDataTable* CharacterMontageDataTable;

	UPROPERTY(Category = Character, EditAnywhere, BlueprintReadWrite)
		class UBoxComponent* _RightFootBox = nullptr;

	UPROPERTY(Category = Character, EditAnywhere, BlueprintReadWrite)
		class UBoxComponent* LeftFootBox = nullptr;

	
private:
	bool IsSprinting = false;
	int ComboIndex = 0;
	bool CanAttack = true; // 处于不同场合或动作判定是否可以攻击
	bool CanCombo = false;  // 是否可以连击
	float CurActionCold = 0;
	class ABaseMonster *CurBoss;

	
	UFUNCTION()
		void  BaseAction(EActionType ActionType, int ActiveIndex);
	UFUNCTION()
		void  NormalAttack();
	UFUNCTION()
		void  N2Attack();
	
	
	
	
	
	
	UFUNCTION()
	FString GetAbilityFilePrefix();
	UFUNCTION()
	FString GetWeaponMontagePrefix();
	UFUNCTION()
	FString GetActionTypeString(EActionType ActionType);





protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	FTimerHandle ComboWaitTimer;
	FTimerHandle StepSoundTimer;
	FTimerHandle InvincibleTimer;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FCharacterAttributes Attributes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ECharacterAbility AbilityType; // 职业类型

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString CurWeaponID = "0";

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int ActionActiveIndex = 10;	// 动作激活标记

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int Defense = 0;	// 动作激活标记

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float BaseAttackActionCold = 0.2f;		// 根据职业不同，可调整基础攻击动作硬直时间
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float ExpressionActionCold = 1.0f;		// 表情动作动作硬直时间

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
		float ComboWaittime = 0.3f; // 连击等待时间
	






public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveForward(float Amount);
	void MoveRight(float Amount);
	void Turn(float Amount);
	void Lookup(float Amount);
	void Sprinting();
	void StopSprinting();


public:
	void CreateFootStepBoxs();
	UFUNCTION()
	void PlayFootStepSound(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void CalBaseAbility();

	UFUNCTION()
	void LoadWeapon();

	UFUNCTION()
	void LoadBaseHUD();



	UFUNCTION()
	void Regen(float DeltaTime);

	UFUNCTION()
	bool CanAction();
	
	UFUNCTION()
	void SetCurActionCold(float num);

	
	UFUNCTION()
	void OnAttackChanged(bool Enable);

	UFUNCTION()
	void OnAttackDamageEnableChanged(bool Enable);

	UFUNCTION()
	void OnAttackComboEnableChanged(bool Enable);

	UFUNCTION()
	void ComboTimeout();

	UFUNCTION()
	void DelHealth(float Health); // 消耗生命值

	UFUNCTION()
	void DelStamina(float Stamina); // 消耗体力

	UFUNCTION()
	void ControlMovement();

	UFUNCTION(BlueprintCallable)
	void SetCurBoss(ABaseMonster* Monster);  // 设置当前Boss;

	UFUNCTION(BlueprintCallable)
	ABaseMonster* GetCurBoss();

	UFUNCTION(BlueprintCallable)
		void AcceptDamage(float Damage, float Penetrate=0);
	UFUNCTION(BlueprintImplementableEvent)
		void BP_AcceptDamage();
	
	UFUNCTION(BlueprintCallable)
		void SetInvincible(float Time);

	UFUNCTION(BlueprintCallable)
		bool IsInvincible();

	UFUNCTION(BlueprintCallable)
		bool IsDead();
	UFUNCTION(BlueprintCallable)
		void Dead();
	UFUNCTION(BlueprintImplementableEvent)
		void BP_Dead();
};

