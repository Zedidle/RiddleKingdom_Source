// 基础人物角色，可以指定武器，有更灵活的招式

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Pawns/BaseCreature.h"
#include "Utils/BaseDefinedStructs.h"
#include "Items/Equips/BaseWeapon.h"
#include "Items/Equips/BaseDeputy.h"
#include "BaseCharacter.generated.h"




UCLASS()
class RPGTUTORIAL_API ABaseCharacter : public ABaseCreature
{
GENERATED_BODY()

public:
	ABaseCharacter();
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<EEquipType, FName> EquipTypeToSocketName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<EEquipType, FString> EquipTypeToEquipID;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<EEquipType, ABaseEquip*> EquipTypeToEquip;


private:
	virtual FString GetActionString(FString ActionKey) override; 

	virtual void N1Attack() override;
	virtual void N2Attack() override;
	virtual void UseDeputy() override;
	virtual void MoveForward(float Amount) override;
	virtual void MoveRight(float Amount) override;


	void BuildEquipTypeToX();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	FTimerHandle ComboWaitTimer;
	FTimerHandle DeputyComboWaitTimer;
	FTimerHandle InvincibleTimer;

	

public:
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


public:

	UFUNCTION(BlueprintCallable)
	void LoadAllEquip();


	UFUNCTION(BlueprintCallable)
		void LoadEquip(FString EquipID = "0", EEquipType EquipType = EEquipType::E_None);
	UFUNCTION(BlueprintCallable)
		void LoadWeapon(FString WeaponID = "0");
	UFUNCTION(BlueprintCallable)
		void LoadDeputy(FString DeputyID = "0");

	
	UFUNCTION()
	void OnAttackChanged(bool Enable);
	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnAttackChanged(bool Enable);


	UFUNCTION()
	void OnAttackDamageEnableChanged(bool Enable);
	UFUNCTION()
	void OnAttackComboEnableChanged(bool Enable);


	UFUNCTION()
		void OnDeputyUse(bool Enable);
	UFUNCTION()
		void OnDeputyDamageEnableChanged(bool Enable);
	UFUNCTION()
		void OnDeputyComboEnableChanged(bool Enable);


	virtual bool ActionDelStamina(FString ActionName);


	UFUNCTION()
	void ComboTimeout();
	UFUNCTION()
	void DeputyComboTimeout();
	

	virtual float AcceptDamage(float Damage, float Penetrate = 0) override;
	virtual void Dead(bool bClearHealth = true) override;


	UFUNCTION(BlueprintCallable)
		void Stiff(float StiffMulti);
	UFUNCTION(BlueprintImplementableEvent)
		void BP_Stiff();


	UFUNCTION(BlueprintCallable)
		bool AbilityCanUseWeaponType(EWeaponType WeaponType);

	UFUNCTION(BlueprintCallable)
		bool AbilityCanUseDeputyType(EDeputyType DeputyType);
};

