// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/InputComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "UObject/UObjectGlobals.h"
#include "Perception/PawnSensingComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/BoxComponent.h"
#include "Engine/EngineTypes.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "TimerManager.h"

#include "Pawns/BaseCharacter.h"
#include "Settings/BaseGameInstance.h"
#include "Animations/BaseAnimInstance.h"
#include "Surroundings/BaseGround.h"
#include "Sound/SoundBase.h"
#include "Utils/BaseDefinedStructs.h"
#include "Items/Equips/BaseWeapon.h"
#include "Pawns/BaseMonster.h"
#include "ActorComponents/BaseInteractive.h"
#include "Pawns/BaseNpc.h"
#include "Skills/BaseSkill.h"
#include "Utils/Util.h"

using Math = UKismetMathLibrary;


// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetCapsuleComponent()->SetCapsuleSize(42,96,true);
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->AirControl = 0.1;

	// 攀爬的速度
	GetCharacterMovement()->MaxFlySpeed = 200;
	GetCharacterMovement()->BrakingDecelerationFlying = 300;
	
	bUseControllerRotationYaw = false;

	Faction =  EFaction::E_Character; // 设置阵营

	BuildEquipTypeToX();
}


void ABaseCharacter::LoadAllEquip()
{
	UE_LOG(LogTemp, Warning, TEXT("ABaseCharacter::LoadAllEquip "));

	for (auto& Entry : EquipTypeToEquipID)
	{

		FPlatformMisc::LocalPrint(
			*FString::Printf(
				TEXT("LoadEquip (%d, \"%s\")\n"),
				Entry.Key,
				*Entry.Value
			)
		);

		LoadEquip(Entry.Value, Entry.Key);
	}
}


void ABaseCharacter::LoadEquip(FString EquipID, EEquipType EquipType)
{
	UBaseGameInstance* GameInstance = Cast<UBaseGameInstance>(GetGameInstance());
	if (!IsValid(GameInstance)) return;
	if(EquipID=="0" || EquipType==EEquipType::E_None) return;
	UDataTable* DT_Equip;
	FName RowName = FName(*EquipID);
	FString ContextString;
	FEquip* Row = nullptr;
	if (EquipType == EEquipType::E_MainHand)
	{
		DT_Equip = GameInstance->DT_Weapon;
		Row = DT_Equip->FindRow<FWeapon>(RowName, ContextString);
	}
	else if(EquipType == EEquipType::E_OffHand)
	{
		DT_Equip = GameInstance->DT_Deputy;
		Row = DT_Equip->FindRow<FDeputy>(RowName, ContextString);

	}
	// 防具,后面再处理
	else
	{
		// DT_Equip = GameInstance->DT_Armor;
	}
	// 再找到对应类型，通过EquipTypeToSocketName放到不同部位;
	// 最后再微调位置，不过最好由美术或建模处理好;

	if (Row != nullptr)
	{
		TSubclassOf<ABaseEquip> C_SubEquip = LoadClass<ABaseEquip>(nullptr, *Row->ActorPath);
		if (C_SubEquip != nullptr)
		{
			ABaseEquip* Equip = EquipTypeToEquip.FindRef(EquipType);
			if (IsValid(Equip)) {
				Equip->Destroy();
			}
			Equip = GetWorld()->SpawnActor<ABaseEquip>(C_SubEquip);
			if (Equip)
			{
				FAttachmentTransformRules Rules = FAttachmentTransformRules(EAttachmentRule::KeepRelative, false);
				Equip->OnEquip(this);
				Equip->AttachToComponent(GetMesh(), Rules, EquipTypeToSocketName.FindRef(EquipType));
				// 将装备的所有属性附加到角色上


				EquipTypeToEquip.Emplace(EquipType, Equip);
			}
		}
	}
}




void ABaseCharacter::LoadWeapon(FString WeaponID)
{
	LoadEquip(WeaponID, EEquipType::E_MainHand);
}

void ABaseCharacter::LoadDeputy(FString DeputyID)
{
	LoadEquip(DeputyID, EEquipType::E_OffHand);
}



void ABaseCharacter::OnAttackChanged(bool Enable)
{
	if (Enable)
	{
		CanAction = false;
	}
	else
	{	
		CanAction = true;
		UWorld* World = GetWorld();
		if (World) {
			World->GetTimerManager().ClearTimer(ComboWaitTimer);
			World->GetTimerManager().SetTimer(ComboWaitTimer, this, &ABaseCharacter::ComboTimeout, ComboWaittime);
		}
	}
	BP_OnAttackChanged(Enable);
}

void ABaseCharacter::OnAttackDamageEnableChanged(bool Enable)
{
	//UE_LOG(LogTemp, Warning, TEXT("ABaseCharacter::OnDeputyComboEnableChanged"));
	ABaseEquip* Equip = EquipTypeToEquip.FindRef(EEquipType::E_MainHand);
	ABaseWeapon* Weapon = nullptr;
	if (IsValid(Equip))
	{
		Weapon = Cast<ABaseWeapon>(Equip);
	}

	if (Weapon)
	{
		if (Enable)
		{
			Weapon->OnUseStart();
		}
		else
		{
			Weapon->OnUseEnd();
		}
	}
}

void ABaseCharacter::OnAttackComboEnableChanged(bool Enable)
{
}


void ABaseCharacter::OnDeputyUse(bool Enable)
{
	UE_LOG(LogTemp, Warning, TEXT("ABaseCharacter::OnDeputyUse"));

	ABaseEquip* Equip = EquipTypeToEquip.FindRef(EEquipType::E_OffHand);
	ABaseDeputy* Deputy = nullptr;

	if (IsValid(Equip))
	{
		Deputy = Cast<ABaseDeputy>(Equip);
	}
	if (Deputy)
	{
		if (Enable)
		{
			Deputy->OnUseStart();
		}
		else
		{
			Deputy->OnUseEnd();
		}
	}
}

void ABaseCharacter::OnDeputyDamageEnableChanged(bool Enable)
{
	UE_LOG(LogTemp, Warning, TEXT("ABaseCharacter::OnDeputyDamageEnableChanged"));
}

void ABaseCharacter::OnDeputyComboEnableChanged(bool Enable)
{
	UE_LOG(LogTemp, Warning, TEXT("ABaseCharacter::OnDeputyComboEnableChanged"));
}

bool ABaseCharacter::ActionDelStamina(FString ActionName)
{
	ABaseEquip* Equip = EquipTypeToEquip.FindRef(EEquipType::E_MainHand);
	ABaseWeapon* Weapon = nullptr;
	if (IsValid(Equip))
	{
		Weapon = Cast<ABaseWeapon>(Equip);
	}

	if (IsValid(Weapon))
	{
		float N1Multi = 1, N2Multi = 1;
		switch (Weapon->WeaponType)
		{
		case EWeaponType::E_Bow:
		{
			N1Multi = 2, N2Multi = 5;
			break;
		}
		case EWeaponType::E_GreatSword:
		{
			N1Multi = 1, N2Multi = 1.5;
			break;
		}
		}

		UE_LOG(LogTemp, Warning, TEXT("ABaseCreature::ActionDelStamina Weapon IsValid"));
		if (ActionName == "N1Attack")
		{
			return DelStamina(Weapon->BaseDelStamina * N1Multi);
		}
		else if (ActionName == "N2Attack")
		{
			return DelStamina(Weapon->BaseDelStamina * N2Multi);
		}
	}

	return Super::ActionDelStamina(ActionName);
}

void ABaseCharacter::ComboTimeout()
{
}

void ABaseCharacter::DeputyComboTimeout()
{
}


FString ABaseCharacter::GetActionString(FString ActionKey)
{
	FString ActionString = Super::GetActionString(ActionKey);
	ABaseEquip* Equip = EquipTypeToEquip.FindRef(EEquipType::E_MainHand);
	ABaseWeapon* Weapon = nullptr;
	if (IsValid(Equip))
	{
		Weapon = Cast<ABaseWeapon>(Equip);
		if (IsValid(Weapon))
		{
			ActionString += Util::GetWeaponTypeString(Weapon->WeaponType, "_");
		}
	}
	return ActionString;
}

void ABaseCharacter::N1Attack()
{	
	ABaseEquip* Equip = EquipTypeToEquip.FindRef(EEquipType::E_MainHand);
	ABaseWeapon* Weapon = nullptr;
	float PlayRate = 1;
	if (IsValid(Equip))
	{
		Weapon = Cast<ABaseWeapon>(Equip);
		PlayRate = Weapon->BasePlayRate;
	}
	FString ActionString = GetActionString("N1Attack");
	PlayMontage(ActionString, "", PlayRate);
}

void ABaseCharacter::N2Attack()
{
	ABaseEquip* Equip = EquipTypeToEquip.FindRef(EEquipType::E_MainHand);
	ABaseWeapon* Weapon = nullptr;
	if (IsValid(Equip))
	{
		Weapon = Cast<ABaseWeapon>(Equip);
	}
	if (!IsValid(Weapon))
	{
		PlayMontage("N2Attack");
	}
	else
	{
		FString ActionString = "N2Attack" + GetMovementModeString("_") + GetSubMovementModeString("_") +
			Util::GetWeaponTypeString(Weapon->WeaponType, "_");
		UE_LOG(LogTemp, Warning, TEXT("ABaseCharacter::N2Attack %f"), Weapon->BasePlayRate);
		PlayMontage(ActionString, "", Weapon->BasePlayRate);
	}
}

void ABaseCharacter::UseDeputy()
{
	ABaseEquip* Equip = EquipTypeToEquip.FindRef(EEquipType::E_OffHand);
	ABaseDeputy* Deputy = nullptr;
	if (IsValid(Equip))
	{
		Deputy = Cast<ABaseDeputy>(Equip);
	}
	if (!IsValid( Deputy)) return;

	FString ActionString = "UseDeputy" + Util::GetDeputyTypeString(Deputy->GetDeputyType(), "_");
	PlayMontage(ActionString);
}


// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	SetMovement(2, 2);
	LoadAllEquip();
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (IsDead()) return;
}


void ABaseCharacter::MoveForward(float Amount)
{
	Super::MoveForward(Amount);

}

void ABaseCharacter::MoveRight(float Amount)
{
	Super::MoveRight(Amount);


}

void ABaseCharacter::BuildEquipTypeToX()
{
	// SocketName
	EquipTypeToSocketName.Add(EEquipType::E_MainHand, "WeaponHold");
	EquipTypeToSocketName.Add(EEquipType::E_OffHand, "DeputyHold");


	// EquipID
	EquipTypeToEquipID.Add(EEquipType::E_MainHand, "0");
	EquipTypeToEquipID.Add(EEquipType::E_OffHand, "0");



}
 

float ABaseCharacter::AcceptDamage(float Damage, float Penetrate)
{
	float TrueDamage = Super::AcceptDamage(Damage, Penetrate);
	if (TrueDamage/GetMaxHealth() > StiffPercent) {
		Stiff(Math::Min(TrueDamage / GetMaxHealth() / StiffPercent, 2));
	}
	return TrueDamage;
}


void ABaseCharacter::Dead(bool bClearHealth)
{
	Super::Dead(bClearHealth);
}

void ABaseCharacter::Stiff(float StiffMulti)
{
	BP_Stiff();
	PlayMontage("Stiff");
}


bool ABaseCharacter::AbilityCanUseWeaponType(EWeaponType WeaponType)
{
	return Util::AbilityCanUseWeaponType(AbilityType, WeaponType);
}

bool ABaseCharacter::AbilityCanUseDeputyType(EDeputyType DeputyType)
{
	return Util::AbilityCanUseDeputyType(AbilityType, DeputyType);
}
