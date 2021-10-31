// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "BaseSaveGame.generated.h"



USTRUCT(BlueprintType)
struct FCreatureData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString CreatureName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector WorldLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FRotator Rotation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EFaction Faction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBaseAttributeSet* BaseAttributeSet = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bPlayerControlling;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bBeenControlled;
};

UCLASS()
class RPGTUTORIAL_API UBaseSaveGame : public USaveGame
{
	GENERATED_BODY()
	
	UBaseSaveGame();

	public:
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FCreatureData> CreatureDatas;

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FString> CreatureUsed;

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			EGameProgress GameProgress = EGameProgress::E_StartMenu;

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			FSetting Setting;

	public:

		UFUNCTION(BlueprintCallable)
		UBaseSaveGame* SaveCreatureData();
};
