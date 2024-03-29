// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Items/Equips/BaseEquip.h"
#include "Utils/BaseDefinedStructs.h"
#include "BaseDeputy.generated.h"

UCLASS()
class RPGTUTORIAL_API ABaseDeputy : public ABaseEquip
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

	//UFUNCTION(BlueprintImplementableEvent)
	//	void DeputyAttack();

	//UFUNCTION(BlueprintImplementableEvent)
	//	void DeputyStopAttack();

};
