// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseNpc.generated.h"


class UPawnSensingComponent;

UCLASS()
class RPGTUTORIAL_API ABaseNpc : public ABaseCreature
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseNpc();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
		UPawnSensingComponent* PawnSensingComp;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 定义SeePawn的回调
	UFUNCTION()
		void OnSeePawn(APawn* Pawn);
};

