// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCreature.h"
#include "BaseMonster.h"
#include "BaseGameManager.h"
#include "Blueprint/UserWidget.h"
#include "Engine/GameInstance.h"
#include "BaseGameInstance.generated.h"


 // 游戏进程，由于可以转换为int,因此可以直接相互比较
UENUM(BlueprintType, Blueprintable)
enum class EDifficulty :uint8
{
	// 正常游戏流程，以下顺序不能搞乱，影响它们转换的int值
	E_Simple UMETA(DisplayName = "简单"),
	E_Normal UMETA(DisplayName = "正常"),
	E_Hard UMETA(DisplayName = "困难"),
	E_Hell UMETA(DisplayName = "地狱"),
};



 // 游戏进程，由于可以转换为int,因此可以直接相互比较
UENUM(BlueprintType, Blueprintable)
enum class EGameProgress :uint8
{
	E_None UMETA(DisplayName = "无"),

	// 突发事件



	// 正常游戏流程，以下顺序不能搞乱，影响它们转换的int值
	E_StartMenu UMETA(DisplayName = "开始菜单"),
	E_WhiteCorridor UMETA(DisplayName = "白色长廊"),
	E_AerialLadder UMETA(DisplayName = "空中悬梯"),
	E_DragonManor UMETA(DisplayName = "龙息之地"),
};



USTRUCT(BlueprintType)
struct FSetting
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Value_Music = 0.5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Value_Sound = 0.5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString Value_Res = FString("1920x1080");
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EDifficulty Difficulty = EDifficulty::E_Normal;
};



UCLASS()
class RPGTUTORIAL_API UBaseGameInstance : public UGameInstance
{
	GENERATED_BODY()
	UBaseGameInstance();

	public:
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerInfo")
			FString SettingFileName = FString("_Setting");



	public:
		UPROPERTY(BlueprintReadOnly)
			TArray<FString> CreatureUsed;
		UPROPERTY(BlueprintReadOnly)
			class ABaseMonster* CurBoss = nullptr;
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			EGameProgress GameProgress = EGameProgress::E_StartMenu;
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			FSetting Setting;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerInfo")
			FString PlayerName = FString("Zedidle");
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerInfo")
			int PlayerIndex = 0;







		





	public:
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DataTable")
			class UDataTable* DT_Weapon = nullptr;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataTable")
			class UDataTable* DT_Deputy = nullptr;





	public:
		UPROPERTY(BlueprintReadOnly, Category = "HUD")
		class UUserWidget* ShootAimHUD = nullptr;
		UPROPERTY(BlueprintReadOnly, Category = "HUD")
		class UUserWidget* BossHealthHUD = nullptr;
		UPROPERTY(BlueprintReadOnly, Category = "HUD")
		class UUserWidget* CreatureHUD = nullptr;
		UPROPERTY(BlueprintReadOnly, Category = "HUD")
		class UUserWidget* CreatureInfoHUD = nullptr;
		UPROPERTY(BlueprintReadOnly, Category = "HUD")
		class UUserWidget* EquipInfoHUD = nullptr;
		UPROPERTY(BlueprintReadOnly, Category = "HUD")
		class UUserWidget* DeadHUD = nullptr;
		UPROPERTY(BlueprintReadOnly, Category = "HUD")
		class UUserWidget* KeyDeadHUD = nullptr;
		

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HUD_Classes")
			TSubclassOf<UUserWidget> C_ShootAimHUD = nullptr;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD_Classes")
			TSubclassOf<UUserWidget> C_BossHealth = nullptr;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD_Classes")
			TSubclassOf<UUserWidget> C_CreatureHUD = nullptr;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD_Classes")
			TSubclassOf<UUserWidget> C_CreatureInfoHUD = nullptr;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD_Classes")
			TSubclassOf<UUserWidget> C_EquipInfoHUD = nullptr;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD_Classes")
			TSubclassOf<UUserWidget> C_DeadHUD = nullptr;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD_Classes")
			TSubclassOf<UUserWidget> C_KeyDeadHUD = nullptr;






	public:


		UFUNCTION(BlueprintCallable)
			void SetSetting(FSetting S);

		UFUNCTION(BlueprintCallable)
			void SaveSetting(UBaseSaveGame* SG);
		UFUNCTION(BlueprintCallable)
			UBaseSaveGame* LoadSetting();


		UFUNCTION(BlueprintCallable)
			void SetPlayerData(FString Name, int Index);

		UFUNCTION(BlueprintCallable)
			void SaveGame(UBaseSaveGame* SG, FString Name = "", int Index = 0);

		UFUNCTION(BlueprintCallable)
			UBaseSaveGame* LoadGame(FString Name = "", int Index = -1);

		UFUNCTION(BlueprintCallable)
		void AddCreatureUsed(ABaseCreature* Creature);

		UFUNCTION(BlueprintCallable)
		void PopCreatureUsed();

		UFUNCTION(BlueprintCallable)
		ABaseCreature* GetLastCreatureUsed(int Index = 0);

		UFUNCTION(BlueprintCallable, BlueprintPure)
		ABaseCreature* GetCurCreatureUsed();



		UFUNCTION(BlueprintCallable)
		void RefreshAllHUD(bool bShow);
		
		UFUNCTION(BlueprintCallable)
		void ShowShootAimHUD(bool bShow);

		UFUNCTION(BlueprintCallable)
		void SetCurBoss(ABaseMonster* M);

		UFUNCTION(BlueprintCallable)
		ABaseMonster* GetCurBoss();
		
		UFUNCTION(BlueprintCallable)
		void ShowCreatureHUD(bool bShow);

		UFUNCTION(BlueprintCallable)
		void ShowCreatureInfo(bool bShow);

		UFUNCTION(BlueprintCallable)
		void ShowEquipInfo(bool bShow);

		UFUNCTION(BlueprintCallable)
		void ShowDeadHUD(bool bShow);

		UFUNCTION(BlueprintCallable)
		void ShowKeyDeadHUD(bool bShow);


		UFUNCTION(BlueprintCallable)
		void SetGmaeProgress(EGameProgress Progress);
};
