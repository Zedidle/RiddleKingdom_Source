// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCreature.h"
#include "BaseDefinedStructs.h"
#include "BaseWeapon.h"
#include "BaseDeputy.h"
#include "BaseCharacter.generated.h"



UCLASS()
class RPGTUTORIAL_API ABaseCharacter : public ABaseCreature
{
GENERATED_BODY()

public:
	ABaseCharacter();




	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UDataTable* WeaponTable = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AActor* CommunicateActor = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class ABaseWeapon* Weapon = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class ABaseDeputy* Deputy = nullptr;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
	//	UDataTable* CharacterMontageDataTable;

	UPROPERTY(Category = Character, EditAnywhere, BlueprintReadWrite)
		class UBoxComponent* RightFootBox = nullptr;

	UPROPERTY(Category = Character, EditAnywhere, BlueprintReadWrite)
		class UBoxComponent* LeftFootBox = nullptr;

private:
	// 战斗部分
	//int ComboIndex = 0;
	float CurActionCold = 0;

	bool CanMove = true;
	bool CanAttack = true; // 处于不同场合或动作判定是否可以攻击
	bool CanPlayMontage = true;  // 是否可以播放蒙太奇

	bool CanCombo = false;  // 是否可以连击

	int DeputyComboIndex = 0;
	bool CanDeputyAttack = true; // 是否能够使用副手
	bool CanDeputyCombo = false;

	int StiffComboIndex = 0; // 被Combo到N次时,动作不一样（可能倒地）

	class ABaseMonster *CurBoss;



private:
	virtual void N1Attack() override;
	virtual void N2Attack() override;
	virtual void UseDeputy() override;
	virtual void MoveForward(float Amount) override;
	virtual void MoveRight(float Amount) override;
	virtual void Turn(float Amount) override;
	virtual void LookUp(float Amount) override;



	UFUNCTION()
	FString GetAbilityFilePrefix();
	//UFUNCTION()
	//FString GetWeaponTypeString(FString Prefix = "");
	//UFUNCTION()
	//FString GetDeputyTypeString(FString Prefix = "");
	UFUNCTION()
	FString GetActionTypeString(EActionType ActionType);





protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	FTimerHandle ComboWaitTimer;
	FTimerHandle DeputyComboWaitTimer;
	FTimerHandle StepSoundTimer;
	FTimerHandle InvincibleTimer;
	FTimerHandle StiffTimer;
	

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FCharacterAttributes Attributes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECharacterAbility AbilityType; // 职业类型


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString CurWeaponID = "0";
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString CurDeputyID = "0";



	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float BaseAttackActionCold = 0.2f;		// 根据职业不同，可调整基础攻击动作硬直时间
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float BaseDeputyActionCold = 0.3f;		// 根据职业不同，可调整基础攻击动作硬直时间
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float BaseStiffToActionCold = 0.1f;		// 根据抗硬直不同，可调整硬直时间
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float ExpressionActionCold = 1.0f;		// 表情动作动作硬直时间


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float ComboWaittime = 0.3f; // 连击等待时间
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float DeputyComboWaittime = 0.2f; // 连击等待时间


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;



public:
	void CreateFootStepBoxs();
	UFUNCTION()
	void PlayFootStepSound(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void CalBaseAbility();

	UFUNCTION()
		void LoadEquip();
	UFUNCTION(BlueprintCallable)
		void LoadWeapon(FString WeaponID = "0");
	UFUNCTION(BlueprintCallable)
		void LoadDeputy(FString DeputyID = "0");


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
		//void OnDeputyChanged(bool Enable);
		void OnDeputyUse(bool Enable);
	UFUNCTION()
		void OnDeputyDamageEnableChanged(bool Enable);
	UFUNCTION()
		void OnDeputyComboEnableChanged(bool Enable);


	UFUNCTION()
	void ComboTimeout();
	UFUNCTION()
	void DeputyComboTimeout();
	
	UFUNCTION()
	void DelHealth(float Health); // 消耗生命值

	UFUNCTION()
	void DelStamina(float Stamina); // 消耗体力

	virtual float AcceptDamage(float Damage, float Penetrate = 0) override;
	
	virtual void Dead() override;


	UFUNCTION(BlueprintCallable)
		void Stiff(float StiffMulti);
	UFUNCTION(BlueprintImplementableEvent)
		void BP_Stiff();



	UFUNCTION(BlueprintCallable)
		void SetCommunicateActor(AActor *Actor);

	UFUNCTION(BlueprintCallable)
		bool IsStopTurn();

};

