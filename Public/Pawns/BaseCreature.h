// 基础生物特点

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"
#include "GameFramework/Character.h"
#include "Utils/BaseDefinedStructs.h"
#include "Items/Equips/BaseWeapon.h"
#include "Items/Equips/BaseDeputy.h"
#include "Actions/ActionInterface.h"

#include "GameplayTagContainer.h"
#include "AbilitySystemInterface.h" 
#include "BaseCreature.generated.h"

class UAbilitySystemComponent;
class UGameplayAbility;
class UBaseAttributeSet;

class UCreatureWidgetComponent;
class USpringArmComponent;
class UCameraComponent;
class UCreatureTracer;
class ABaseCreature;
class ABaseSkill;


UCLASS()
class RPGTUTORIAL_API ABaseCreature : public ACharacter, public IActionInterface, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCreature();
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UCreatureWidgetComponent* HealthWidget = nullptr;
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		USpringArmComponent* SpringArm = nullptr;
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* Camera = nullptr;
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UCreatureTracer* CreatureTracer = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
		UDataTable* CreatureMontageDataTable;

	TMap<FString, int> ComboIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<ABaseSkill*> SkillsOnUsing;

	UFUNCTION(BlueprintCallable)
	void AddSkillOnUsing(ABaseSkill* Skill);

	UPROPERTY(BlueprintReadOnly)
	ABaseCreature* IntrudingTarget = nullptr;


private:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;


public:


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UAbilitySystemComponent* AbilitySystemComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UBaseAttributeSet* BaseAttributeSet;

	UFUNCTION(BlueprintCallable)
		void GetAbility(TSubclassOf<UGameplayAbility> AbilityType);

	UFUNCTION(BlueprintCallable, BlueprintPure)
		virtual FString GetActionString(FString ActionKey);
	
	UPROPERTY(BlueprintReadWrite)
	   FGameplayTag ActiveEventTag = FGameplayTag::EmptyTag;

public:
	// 是否为关卡关键角色，需要放到场景后再设置
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bLevelKey = false;  

	// true: 将角色和镜头的转向锁定到Target
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bLocking = false;

	// false: 用于适配没有打横走（只有1D AnimBS）的角色，
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bFaceToTarget = true;  

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float UnitDistance = 100; // 最近的攻击距离

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bBeenControlled = false; // 是否被玩家控制过

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector PosStartInWorld = FVector(); // 放置在世界才有用到，在Begin获取其当前位置来设置，用于Reset重置

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MaxLookupPitch = 80;  // 由于每个Creature的大小都不同，需要在蓝图独自设定
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MinLookupPitch = -80;  // 由于每个Creature的大小都不同，需要在蓝图独自设定

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float BaseSpringArmLength = 400;  // 由于每个Creature的大小都不同，需要在蓝图独自设定

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float CommunicatePressTime = 0;
	// 是否作为AI
	UPROPERTY(EditAnywhere, BlueprintReadWrite) 
		bool bAI = false;

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
	UFUNCTION(BlueprintCallable)
		virtual void JumpPress();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void BP_JumpPress();

	UFUNCTION(BlueprintCallable)
		virtual void Sprinting();
	UFUNCTION(BlueprintCallable)
		virtual void StopSprinting();

	UFUNCTION(BlueprintCallable)
		virtual void N1Attack();
	UFUNCTION(BlueprintCallable)
		virtual void N2Attack();

	UFUNCTION(BlueprintCallable)
		virtual void UseDeputy();

	UFUNCTION(BlueprintCallable)
		virtual void Lock();
	UFUNCTION(BlueprintCallable)
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


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ABaseSkill> C_SpriteIntrudePawn = nullptr;

	UFUNCTION(BlueprintPure)
	bool CanBeIntrude(ABaseCreature* Intruder);

	// 是否已消亡
	UFUNCTION(BlueprintPure)
	bool IsTrueDead(); 

	// 基础变量设置
	UPROPERTY(BlueprintReadOnly)
	float DeltaSeconds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bSprinting = false;

	bool CanAction = true; // 处于不同场合或动作判定是否可以攻击


	// 计时器部分
	FTimerHandle DilationTimer;
	FTimerHandle InvincibleTimer;







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
	float Time_MoveToTarget = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EFaction Faction = EFaction::E_None;

	// 是否显示准心,随时都要入侵
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bShowShootAnim = true; 



	// 战斗时是否给对方显示血条
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bShowBlood = false;

	// 是否显示状态栏，影响复活后的角色界面，也是可以直接绑定。
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool ShowHUD = true;


	// 基础生命部分
	UFUNCTION(BlueprintCallable, BlueprintPure)
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
	UFUNCTION(BlueprintCallable, BlueprintPure)
		bool IsFalling();
	UFUNCTION(BlueprintCallable, BlueprintPure)
		bool IsFlying();
	UFUNCTION(BlueprintCallable, BlueprintPure)
		bool IsGround();
	UFUNCTION(BlueprintCallable, BlueprintPure)
		bool IsSwimming();
	UFUNCTION(BlueprintCallable, BlueprintPure)
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
		void SetCurHealth(float Value);
	UFUNCTION(BlueprintCallable)
		void SetCurStamina(float Value);
	UFUNCTION(BlueprintCallable)
		void SetCurMana(float Value);

	UFUNCTION(BlueprintCallable, BlueprintPure)
		float GetCurHealth();
	UFUNCTION(BlueprintCallable, BlueprintPure)
		float GetCurStamina();
	UFUNCTION(BlueprintCallable, BlueprintPure)
		float GetCurMana();

	UFUNCTION(BlueprintCallable, BlueprintPure)
		float GetMaxHealth();
	UFUNCTION(BlueprintCallable, BlueprintPure)
		float GetMaxStamina();
	UFUNCTION(BlueprintCallable, BlueprintPure)
		float GetMaxMana();

	UFUNCTION(BlueprintCallable, BlueprintPure)
		float GetHealthPercent();
	UFUNCTION(BlueprintCallable, BlueprintPure)
		float GetStaminaPercent();
	UFUNCTION(BlueprintCallable, BlueprintPure)
		float GetManaPercent();

	UFUNCTION(BlueprintCallable)
		bool DelHealth(float Value);
	UFUNCTION(BlueprintCallable)
		bool DelStamina(float Value);
	UFUNCTION(BlueprintCallable)
		bool DelMana(float Value);


	// 设置/判断 无敌
	UFUNCTION(BlueprintCallable)
		void SetInvincible(float Time, bool bForce=false);
	UFUNCTION(BlueprintCallable, BlueprintPure)
		bool IsInvincible();



	UFUNCTION(BlueprintCallable)
		virtual void SetTarget(ABaseCreature* C);
	UFUNCTION(BlueprintCallable)
		ABaseCreature* GetTarget();

	UFUNCTION(BlueprintCallable)
		void IntrudeTarget(ABaseCreature* Creature);
	UFUNCTION(BlueprintImplementableEvent)
		void BP_IntrudeTarget();


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
	UFUNCTION(BlueprintCallable)
		void Tick_MoveToTarget(float Time = 0, float SpeedMulti=2);
	UFUNCTION(BlueprintImplementableEvent)
		void BP_Tick_MoveToTarget();

	UFUNCTION()
		void Tick_CalFalling();




	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
		EDistance GetDistanceTypeToTarget();

	UFUNCTION()
		virtual bool ActionDelStamina(FString ActionName);

	UFUNCTION(BlueprintCallable)
		bool PlayMontage(FString ActionString, FString SectionName = "", float PlayRate=1.0f);

	UFUNCTION(BlueprintCallable, BlueprintPure)
		bool NeedQuickRotate();

	UFUNCTION(BlueprintPure)
		FTransform GetTransform_ProjectileToTarget(ABaseCreature* _Target=nullptr, FName StartSocketName = "Fire_Start");
};
