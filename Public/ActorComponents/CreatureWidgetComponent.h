// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "CreatureWidgetComponent.generated.h"

/**
 * 
 */
UCLASS()
class RPGTUTORIAL_API UCreatureWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

	virtual void InitWidget() override;

	public:
		virtual void BeginPlay() override;
};
