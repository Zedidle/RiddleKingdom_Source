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

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void ActionFlatSNear();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void ActionFlatNear();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void ActionFlatMid();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void ActionFlatFar();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void ActionFlatSFar();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void ActionPlumbNear();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void ActionPlumbMid();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void ActionPlumbFar();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void ActionPlumbSFar();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void ActionTurn90();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void ActionTakeOff();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void ActionRandomWander();

};
