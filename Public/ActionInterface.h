// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "../Base/BaseDefinedStructs.h"
#include "ActionInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UActionInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class RPGTUTORIAL_API IActionInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void ActionDelegate(EDistance D);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void ActionFlatNear();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void ActionFlatMid();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void ActionFlatFar();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void ActionFlatSFar();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void ActionPlumbNear();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void ActionPlumbMid();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void ActionPlumbFar();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void ActionPlumbSFar();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void ActionTurn90();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void ActionTakeOff();

};
