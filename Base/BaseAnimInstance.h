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
	bool IsFalling;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int CharacterActionActiveIndex;

public:


	UFUNCTION(BlueprintCallable)
		void UpdateAnimaton();

};
