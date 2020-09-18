// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BaseAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class RPGTUTORIAL_API UBaseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Direction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ForwardSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RightSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float UpSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsFalling;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsFlying;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsLocking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsClimbing;

public:


	UFUNCTION(BlueprintCallable)
		void UpdateAnimaton();
	UFUNCTION(BlueprintImplementableEvent)
		void BP_UpdateAnimaton();

	UFUNCTION(BlueprintCallable)
		void NativeBeginPlay();

	//UFUNCTION(BlueprintCallable)
	//	void MontageStart(UAnimInstance* Montage);

	//UFUNCTION(BlueprintCallable)
	//	void MontageEnd(UAnimMontage* Montage, bool bInterrupted);
};
