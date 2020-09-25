// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseDefinedStructs.h"
#include "BaseDeputy.generated.h"

UCLASS()
class RPGTUTORIAL_API ABaseDeputy : public AActor
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	ABaseDeputy();
private:
	/** The main skeletal mesh associated with this Character (optional sub-object). */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* SkeletalMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* StaticMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UArrowComponent* ArrowComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class ABaseCreature* User;
public:


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EDeputyType DeputyType = EDeputyType::E_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DelStamina = 10;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	EDeputyType GetDeputyType();
	
	UFUNCTION(BlueprintCallable)
	void SetUser(ABaseCreature* U);
	UFUNCTION(BlueprintCallable)
	ABaseCreature* GetUser();

	//UFUNCTION(BlueprintImplementableEvent)
	//	void DeputyAttack();

	//UFUNCTION(BlueprintImplementableEvent)
	//	void DeputyStopAttack();


	UFUNCTION(BlueprintCallable)
		void UseDeputy_Start();
	UFUNCTION(BlueprintImplementableEvent)
		void BP_UseDeputy_Start();

	UFUNCTION(BlueprintCallable)
		void UseDeputy_End();
	UFUNCTION(BlueprintImplementableEvent)
		void BP_UseDeputy_End();
};
