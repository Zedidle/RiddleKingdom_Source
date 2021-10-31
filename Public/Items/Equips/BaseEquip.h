// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseEquip.generated.h"

UCLASS()
class RPGTUTORIAL_API ABaseEquip : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseEquip();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString EquipID = "000";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true"))
		class ABaseCharacter* User = nullptr;




protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;




public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	virtual void OnEquip(ABaseCharacter* EquipUser);
	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnEquip(ABaseCharacter* EquipUser);

	UFUNCTION(BlueprintCallable)
		virtual void OnUseStart();
	UFUNCTION(BlueprintImplementableEvent)
		void BP_OnUseStart();

	UFUNCTION(BlueprintCallable)
		virtual void OnUseEnd();
	UFUNCTION(BlueprintImplementableEvent)
		void BP_OnUseEnd();
};
