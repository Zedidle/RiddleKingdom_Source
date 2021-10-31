// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "BaseGameplayEffectExecutionCalc.generated.h"

/**
 * 
 */
UCLASS()
class RPGTUTORIAL_API UBaseGameplayEffectExecutionCalc : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
		UBaseGameplayEffectExecutionCalc();
		virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

	
};
