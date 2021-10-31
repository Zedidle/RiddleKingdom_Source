// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "AbilitySystemInterface.h" 
#include "BaseSkill.generated.h"


class UAbilitySystemComponent;

UCLASS()
class RPGTUTORIAL_API ABaseSkill : public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:	
	ABaseSkill();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UAbilitySystemComponent* AbilitySystemComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SkillCore = 1;  // 技能核心, 用于后期根据使用者的属性增加伤害
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseDamage = 1;  // 基础伤害

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<ABaseSkill*> ChildSkills;


private:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;


protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = "true"))
		FGameplayTag EventTag;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = "true"))
		ESkillType SkillType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = "true"))
		ABaseCreature* Target;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FGameplayEventData Payload;


public:	
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable)
		bool MakeDamage(ABaseCreature* _Target = nullptr);
	UFUNCTION(BlueprintCallable)
		EFaction GetFaction();

	UFUNCTION(BlueprintCallable, BlueprintPure)
		ABaseCreature* GetSkillUser();


	UFUNCTION(BlueprintCallable)
	void AddChildSkill(ABaseSkill* Skill);

	virtual void K2_DestroyActor() override;

	UFUNCTION(BlueprintCallable)
	void SendGameplayEventToActor();

	UFUNCTION(BlueprintCallable)
	void GetAbility(TSubclassOf<UGameplayAbility> AbilityType);


};
