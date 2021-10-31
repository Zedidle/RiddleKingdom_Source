// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CreatureTracer.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPGTUTORIAL_API UCreatureTracer : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCreatureTracer();
	class ABaseCreature* Owner;

	UPROPERTY(BlueprintReadOnly)
	class ABaseCreature* Creature = nullptr;
	UPROPERTY(BlueprintReadOnly)
	class ABaseEquip* Equip = nullptr;



	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<ETraceTypeQuery> TraceChannel = ETraceTypeQuery::TraceTypeQuery1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AActor*> ActorsToIgnore;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//TEnumAsByte <EDrawDebugTrace::Type> DrawDebugType = EDrawDebugTrace::Type::None;
	TEnumAsByte <EDrawDebugTrace::Type> DrawDebugType;

	FHitResult HitResult;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TraceLength = 5000;





protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void OnHit(bool bHit);

	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnHit(bool bHit);
		
};