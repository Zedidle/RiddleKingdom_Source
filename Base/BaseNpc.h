// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseNpc.generated.h"

UCLASS()
class RPGTUTORIAL_API ABaseNpc : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseNpc();


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class ABaseCharacter* CurCharacter = nullptr;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void Welcome(ABaseCharacter* Character);


	UFUNCTION(BlueprintCallable)
		void Communicate();

	UFUNCTION(BlueprintImplementableEvent)
		void BP_Communicate();

};
