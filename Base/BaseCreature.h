// 基础生物特点

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/Character.h"
#include "BaseDefinedStructs.h"
#include "BaseWeapon.h"
#include "BaseDeputy.h"
#include "ActionInterface.h"
#include "BaseSkill.h"
#include "BaseCreature.generated.h"


UCLASS()
class RPGTUTORIAL_API ABaseCreature : public ACharacter, public IActionInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCreature();
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UCreatureWidgetComponent* HealthWidget = nullptr;
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* SpringArm = nullptr;
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* Camera = nullptr;
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UCreatureTracer* CreatureTracer = nullptr;

	//FComboIndex ComboIndex;
	TMap<FString, int> ComboIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<ABaseSkill*> SkillsOnUsing;

	UFUNCTION(BlueprintCallable)
	void AddSkillOnUsing(ABaseSkill* Skill);

public:
	// false: 用于适配没有打横走（只有1D AnimBS）的角色，
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bLockToTarget = true;  

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float UnitDistance = 100; // 最近的攻击距离

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bBeenControlled = false; // 是否被玩家控制过

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector PosStartInWorld = FVector(); // 放置在世界才有用到，在Begin获取其当前位置来设置，用于Reset重置

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float BaseSpringArmLength = 400;  // 由于每个Creature的大小都不同，需要在蓝图独自设定

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float CommunicatePressTime = 0;
	// 是否作为AI
	UPROPERTY(EditAnywhere, BlueprintReadWrite) 
		bool bAI = false;

	UFUNCTION(BlueprintCallable)
		bool IsPlayerControlling();

	// 是否处于战斗状态
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bCombating = false;
	// 是否准备起飞，一般由AI处理
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=AI)
		bool bReadyTakeOff = false;

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


	void PossessedBy(AController* NewController) override;

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
		void FarView();
	UFUNCTION()
		void NearView();

	UFUNCTION(BlueprintCallable)
	void ResetSave();
	UFUNCTION(BlueprintImplementableEvent)
	void BP_ResetSave();


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

	

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class ABaseCreature* Target = nullptr;

	class ABaseWeapon* Weapon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<APawn> C_SpriteIntrudePawn = nullptr;

	UFUNCTION(BlueprintCallable)
	bool CanBeIntrude();

	// 基础变量设置
	UPROPERTY(BlueprintReadOnly)
	float DeltaSeconds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsSprinting = false;

	bool CanAction = true; // 处于不同场合或动作判定是否可以攻击


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float CurHealth = 95;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MaxHealth = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float HealthRegen = 1; // 每秒恢复生命值
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float CurStamina = 95;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MaxStamina = 100;
	// 每秒恢复体力
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float StaminaRegen = 1;	
	// 防御力
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int Defense = 5;	

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bWithoutPlayerControlled = false;  // 是否失去了玩家的控制
	UFUNCTION(BlueprintCallable)
		bool IsDead();
	UFUNCTION(BlueprintCallable)
		virtual void Dead(bool bClearHealth = true);
	UFUNCTION(BlueprintImplementableEvent)
		void BP_Dead();

	UFUNCTION(BlueprintCallable)
		virtual void Revive();
	UFUNCTION(BlueprintImplementableEvent)
		void BP_Revive();
	

	

	UFUNCTION(BlueprintCallable)
	virtual	float AcceptDamage(float Damage, float Penetrate = 0);
	UFUNCTION(BlueprintImplementableEvent)
		void BP_AcceptDamage(float TrueDamage);


	UFUNCTION(BlueprintCallable)
		bool SameFaction(ABaseCreature* OtherCreature);


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

	UFUNCTION(BlueprintCallable)
		void IntrudeTarget(ABaseCreature* Creature);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
	void Tick_Regen();
	UFUNCTION()
		void Tick_LockToFaceTarget();
	UFUNCTION()
		void Tick_CalFalling();






	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
		EDistance GetDistanceTypeToTarget();

	UFUNCTION(BlueprintCallable)
		bool PlayMontage(FString Rowname, FString SectionName = "", float PlayRate=1.0f);

	UFUNCTION(BlueprintCallable)
		bool NeedQuickRotate();

	UFUNCTION(BlueprintCallable)
		FTransform GetTransform_ProjectileToTarget();
};
