// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseDefinedStructs.h"
#include "BaseGround.generated.h"





UCLASS()
class RPGTUTORIAL_API ABaseGround : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseGround();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* StaticMesh = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* Box = nullptr;

	// 0:无声 1:草地  2:岩地 3:雪地 4:沙地
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EGroundType GroundType = EGroundType::E_NONE;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FString GroundTypeToTableRowname();
};
