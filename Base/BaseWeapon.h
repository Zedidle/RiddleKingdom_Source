// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseDefinedStructs.h"
#include "BaseWeapon.generated.h"

UCLASS()
class RPGTUTORIAL_API ABaseWeapon : public AActor
{
	GENERATED_BODY()

public:
	ABaseWeapon();


private:
	/** The main skeletal mesh associated with this Character (optional sub-object). */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* SkeletalMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* StaticMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* Box;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseDamage = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWeaponType _WeaponType = EWeaponType::E_NONE;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent)
	void WeaponAttack();

	UFUNCTION(BlueprintImplementableEvent)
	void WeaponStopAttack();


	UFUNCTION()
	EWeaponType GetWeaponType();
};
