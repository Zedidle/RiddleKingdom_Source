// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseEquip.h"
#include "BaseDefinedStructs.h"
#include "BaseWeapon.generated.h"

// 武器定义：只有BaseCharacter类才能使用的，附加动作、增强伤害的道具

UCLASS()
class RPGTUTORIAL_API ABaseWeapon : public ABaseEquip
{
	GENERATED_BODY()

public:
	ABaseWeapon();
	FWeaponAbility Ability;


private:
	/** The main skeletal mesh associated with this Character (optional sub-object). */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* SkeletalMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* StaticMesh;
	

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WeaponCore = 1;  // 武器核心 提升所有能力加成 1 - 3
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
		float DelStamina = 10.f;

	UFUNCTION(BlueprintCallable)
	void UseWeapon_Start();
	UFUNCTION(BlueprintImplementableEvent)
	void BP_UseWeapon_Start();

	UFUNCTION(BlueprintCallable)
	void UseWeapon_End();
	UFUNCTION(BlueprintImplementableEvent)
	void BP_UseWeapon_End();

	UFUNCTION(BlueprintCallable)
		float GetDamage();
	UFUNCTION(BlueprintCallable)
		float MakeDamage(ABaseCreature* Target, float ExtraDamage);

	void SetCurAbility(FCharacterAttributes Attributes);

	UFUNCTION()
	EWeaponType GetWeaponType();
};
