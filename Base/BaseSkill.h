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
	ABaseSkill();


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SkillCore = 1;  // 技能核心, 用于后期根据使用者的属性增加伤害
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseDamage = 1;  // 基础伤害

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<ABaseSkill*> ChildSkills;


protected:
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = "true"))
		ESkillType SkillType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = "true"))
		ABaseCreature* SkillUser;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = "true"))
		ABaseCreature* Target;

public:	
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable)
		bool MakeDamage(ABaseCreature* _Target = nullptr, float ExtraDamage = 0);
	UFUNCTION(BlueprintCallable)
		void SetSkillInfo(ESkillType Type, ABaseCreature* User);
	UFUNCTION(BlueprintCallable)
		EFaction GetFaction();



	UFUNCTION(BlueprintCallable)
	void AddChildSkill(ABaseSkill* Skill);


	virtual void K2_DestroyActor() override;

};
