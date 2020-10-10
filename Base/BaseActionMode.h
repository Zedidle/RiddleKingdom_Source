// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BaseActionMode.generated.h"

/**
 * 
 */
UCLASS()
class RPGTUTORIAL_API UBaseActionMode: public UObject
{
	GENERATED_BODY()
	
	public:
		UFUNCTION(BlueprintImplementableEvent)
		void Action_FlatNear();
	
};
