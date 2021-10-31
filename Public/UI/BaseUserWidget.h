// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Pawns/BaseCreature.h"
#include "BaseUserWidget.generated.h"


UCLASS()
class RPGTUTORIAL_API UBaseUserWidget : public UUserWidget
{
	GENERATED_BODY()


    public:
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class ABaseCreature* Owner = nullptr;

		UFUNCTION(BlueprintCallable)
		void SetOwner(ABaseCreature* Creature);
};
