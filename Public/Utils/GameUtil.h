// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/GameplayStatics.h"
#include "GameUtil.generated.h"

/**
 * 
 */
UCLASS()
class RPGTUTORIAL_API UGameUtil : public UGameplayStatics
{
	GENERATED_BODY()

	FTimerHandle DilationTimer;
	const UWorld* W;

	public:
		UGameUtil();

		void SetDilation(const UWorld* W, float Scale, float Time);
		void DilationTimeout();

	
};
