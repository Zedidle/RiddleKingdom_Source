// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Items/Equips/BaseEquip.h"
#include "Utils/BaseDefinedStructs.h"
#include "BaseWeapon.generated.h"

// 武器定义：只有BaseCharacter类才能使用的，附加动作、增强伤害的道具

UCLASS()
class RPGTUTORIAL_API ABaseWeapon : public ABaseEquip
{
	GENERATED_BODY()

public:
	ABaseWeapon();
	FWeaponAbility Ability;

	// 是否显示准心
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bShowShootAnim = false;

private:
	/** The main skeletal mesh associated with this Character (optional sub-object). */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* SkeletalMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* StaticMesh;
	

public:

	
	// 武器核心 提升所有能力加成 1 - 3
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int WeaponCore = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseDamage = 5;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BasePlayRate = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWeaponType WeaponType = EWeaponType::E_None;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;




public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float BaseDelStamina = 10.f;

	UFUNCTION(BlueprintCallable)
		float GetDamage();
	UFUNCTION(BlueprintCallable)
		float MakeDamage(ABaseCreature* Target, float ExtraDamage);

	//void SetCurAbility(FCharacterAttributes Attributes);

	virtual void OnEquip(ABaseCharacter* EquipUser) override;

	UFUNCTION()
	EWeaponType GetWeaponType();
};
