// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseSkill.generated.h"

UCLASS()
class RPGTUTORIAL_API ABaseSkill : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseSkill();
	ABaseSkill(ESkillSide Side);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SkillCore = 1;  // 技能核心, 用于后期根据使用者的属性增加伤害
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseDamage = 1;  // 基础伤害

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ESkillSide SkillSide;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ESkillType SkillType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AActor* SkillUser;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SetSkillInfo(ESkillSide Side, ESkillType Type,AActor* User);
	UFUNCTION(BlueprintCallable)
	bool MakeDamageToActor(AActor* Actor, float ExtraDamage=0);

};
