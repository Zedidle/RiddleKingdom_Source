// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseDefinedStructs.h"
#include "BaseWeapon.h"
#include "BaseCharacter.generated.h"





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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)  // 方便在编辑器调整大小
		class UBoxComponent* _RightFootBox = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UBoxComponent* _LeftFootBox = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<UUserWidget> BaseHUD = nullptr;

	UPROPERTY()
		class ABaseWeapon* Weapon = nullptr;
		//TSubclassOf<ABaseWeapon> Weapon = nullptr;

private:
	bool IsSprinting = false;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ECharacterAbility AbilityType; // 职业类型

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString CurWeaponID = "0";

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



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void ControlMaxWalkSpeed();


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
	void  BaseAttack();

	UFUNCTION()
	void Regen(float DeltaTime);

};
