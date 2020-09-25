// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "Components/InputComponent.h"
#include "BaseAnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/BoxComponent.h"
#include "Engine/EngineTypes.h"
#include "Components/SkeletalMeshComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "BaseGround.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "BaseDefinedStructs.h"
#include "Blueprint/UserWidget.h"
#include "BaseWeapon.h"
#include "UObject/ConstructorHelpers.h"
#include "UObject/UObjectGlobals.h"
#include "TimerManager.h"
#include "BaseMonster.h"
#include "Kismet/GameplayStatics.h"
#include "BaseInteractive.h"
#include "Perception/PawnSensingComponent.h"
#include "BaseNpc.h"
#include "../Util.h"
#include "Kismet/KismetMathLibrary.h"
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
	
	this->bUseControllerRotationYaw = false;

	CreateFootStepBoxs();
	CalBaseAbility();
	Faction =  EFaction::E_Character; // 设置阵营



	//FString Filename = "DataTable'/Game/Main/Characters/" + GetAbilityFilePrefix() + "Character/CharacterMontage_DataTable.CharacterMontage_DataTable'";
	//static ConstructorHelpers::FObjectFinder<UDataTable> MontageDataTableObject(*Filename);
	//if (MontageDataTableObject.Succeeded()) {
	//	CharacterMontageDataTable = MontageDataTableObject.Object; // 角色动画蒙太奇集
	//}
}


void ABaseCharacter::CalBaseAbility()
{
	// 存档内容，尚未做
	CurWeaponID = "1";
	CurDeputyID = "1";
	CurHealth = 100;
	MaxHealth = 100;
	HealthRegen = 1;
	CurStamina = 100;
	MaxStamina = 100;
	StaminaRegen = 15;

	Attributes.Body = 1;
	Attributes.Power = 1;
	Attributes.Sprit = 1;
	Attributes.Agile = 1;
}

void ABaseCharacter::LoadEquip()
{
	LoadWeapon(CurWeaponID);
	LoadDeputy(CurDeputyID);
}

void ABaseCharacter::LoadWeapon(FString WeaponID)
{
	if (WeaponID == "0" || !IsValid(WeaponTable)) return;
	UE_LOG(LogTemp, Warning, TEXT("ABaseCharacter::LoadWeapon WeaponID:%s"), *CurWeaponID);

	CurWeaponID = WeaponID;
	FName RowName = FName(*CurWeaponID);
	FString ContextString;
	FWeapon* Row = WeaponTable->FindRow<FWeapon>(RowName, ContextString);

	if (Row)
	{	
		TSubclassOf<ABaseWeapon> SubWeaponClass = LoadClass<ABaseWeapon>(nullptr, *Row->WeaponActorPath);
		UE_LOG(LogTemp, Warning, TEXT("Weapon->LoadWeapon 111"));
		if (SubWeaponClass != nullptr)
		{
			if (IsValid(Weapon)) {
				Weapon->Destroy();
			}
			Weapon = GetWorld()->SpawnActor<ABaseWeapon>(SubWeaponClass);
			if (Weapon)
			{
				FAttachmentTransformRules Rules = FAttachmentTransformRules(EAttachmentRule::KeepRelative, false);
				Weapon->SetCurAbility(Attributes);
				Weapon->User = this;
				Weapon->AttachToComponent(GetMesh(), Rules, "WeaponHold");
			}
		}
	}
}

void ABaseCharacter::LoadDeputy(FString DeputyID)
{
	//UDataTable* DeputyTable = LoadObject<UDataTable>(NULL, TEXT("DataTable'/Game/Main/Data/Deputy_DataTable.Deputy_DataTable'"));
	if (DeputyID == "0" || !IsValid(DeputyTable)) return;
	UE_LOG(LogTemp, Warning, TEXT("ABaseCharacter::LoadDeputy DeputyID:%s"), *CurDeputyID);
	CurDeputyID = DeputyID;
	FName RowName = FName(*CurDeputyID);
	FString ContextString;
	FDeputy* Row = DeputyTable->FindRow<FDeputy>(RowName, ContextString);
	if (Row)
	{
		TSubclassOf<ABaseDeputy> SubDeputyClass = LoadClass<ABaseDeputy>(nullptr, *Row->DeputyActorPath);
		if (SubDeputyClass != nullptr)
		{
			if (Deputy) {
				Deputy->Destroy();
			}
			Deputy = GetWorld()->SpawnActor<ABaseDeputy>(SubDeputyClass);
			if (Deputy)
			{
				FAttachmentTransformRules Rules = FAttachmentTransformRules(EAttachmentRule::KeepRelative, false);
				Deputy->AttachToComponent(GetMesh(), Rules, "DeputyHold");
				Deputy->SetUser(this);
			}
		}
	}
}




bool ABaseCharacter::CanAction()
{
	return CurActionCold <= 0;
}

void ABaseCharacter::SetCurActionCold(float num)
{
	CurActionCold = num;
}

void ABaseCharacter::OnAttackChanged(bool Enable)
{
	if (Enable)
	{
		CanAttack = false;
	}
	else
	{	
		CanMove = true;
		CanAttack = true;
		CanPlayMontage = true;
		UWorld* World = GetWorld();
		if (World) {
			World->GetTimerManager().ClearTimer(ComboWaitTimer);
			World->GetTimerManager().SetTimer(ComboWaitTimer, this, &ABaseCharacter::ComboTimeout, ComboWaittime);
		}
	}
}

void ABaseCharacter::OnAttackDamageEnableChanged(bool Enable)
{
	UE_LOG(LogTemp, Warning, TEXT("ABaseCharacter::OnDeputyComboEnableChanged"));
	if (Enable)
	{
		CanMove = false;
		if (IsValid(Weapon))
		{
			Weapon->UseWeapon_Start();
		}
	}
	else
	{
		if (IsValid(Weapon))
		{
			Weapon->UseWeapon_End();
		}
	}
}

void ABaseCharacter::OnAttackComboEnableChanged(bool Enable)
{
	if (Enable)
	{
		CanCombo = true;
		CanAttack = true;
	}
	else
	{
		CanCombo = false;
	}
}


void ABaseCharacter::OnDeputyUse(bool Enable)
{
	UE_LOG(LogTemp, Warning, TEXT("OnDeputyUse 0"));

	if (IsValid(Deputy))
	{
		if (Enable)
		{
			UE_LOG(LogTemp, Warning, TEXT("UseDeputy_Start"));
			Deputy->UseDeputy_Start();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("UseDeputy_End"));
			Deputy->UseDeputy_End();
		}
	}

	//if (Enable)
	//{
	//	CanDeputyAttack = false;
	//}
	//else
	//{
	//	CanDeputyAttack = true;
	//	UWorld* World = GetWorld();
	//	if (World) {
	//		World->GetTimerManager().ClearTimer(DeputyComboWaitTimer);
	//		World->GetTimerManager().SetTimer(DeputyComboWaitTimer, this, &ABaseCharacter::DeputyComboTimeout, DeputyComboWaittime);
	//	}
	//}
}

void ABaseCharacter::OnDeputyDamageEnableChanged(bool Enable)
{
	UE_LOG(LogTemp, Warning, TEXT("OnDeputyDamageEnableChanged"));
	if (Enable)
	{
		if (IsValid(Deputy))
		{
			Deputy->UseDeputy_Start();
		}
	}
	else
	{
		if (IsValid(Deputy))
		{
			Deputy->UseDeputy_End();
		}
	}
}

void ABaseCharacter::OnDeputyComboEnableChanged(bool Enable)
{
	UE_LOG(LogTemp, Warning, TEXT("ABaseCharacter::OnDeputyComboEnableChanged"));

	if (Enable)
	{
		CanDeputyCombo = true;
		CanDeputyAttack = true;
	}
	else
	{
		CanDeputyCombo = false;
	}
}

void ABaseCharacter::ComboTimeout()
{
	if (CanAttack)
	{
		ComboIndex = 0;
	}
}

void ABaseCharacter::DeputyComboTimeout()
{
	if (CanDeputyAttack)
	{
		DeputyComboIndex = 0;
	}
}

void ABaseCharacter::DelHealth(float Health)
{
	CurHealth -= Health;
	CurHealth = Math::FMax(CurHealth, 0);
}

void ABaseCharacter::DelStamina(float Stamina)
{
	CurStamina -= Stamina;
	CurStamina = Math::FMax(CurStamina, 0);
}


void ABaseCharacter::N1Attack()
{	
	if (!IsValid(Weapon)) return;
	if (Weapon->DelStamina > CurStamina) return;
	FString ActionString = "N1Attack" + GetMovementModeString("_") + GetSubMovementModeString("_") + 
		Util::GetWeaponTypeString(Weapon->WeaponType,"_");
	if (PlayMontage(ActionString))
	{
		DelStamina(Weapon->DelStamina);
	}
}

void ABaseCharacter::N2Attack()
{
	if (!IsValid(Weapon)) return;
	if (Weapon->DelStamina > CurStamina) return;

	FString ActionString = "N2Attack" + GetMovementModeString("_") + GetSubMovementModeString("_") + 
		Util::GetWeaponTypeString(Weapon->WeaponType, "_");
	if (PlayMontage(ActionString))
	{
		DelStamina(Weapon->DelStamina);
	}
}

void ABaseCharacter::UseDeputy()
{
	if (!IsValid( Deputy)) return;
	if (Deputy->DelStamina > CurStamina) return;

	FString ActionString = "UseDeputy" + Util::GetDeputyTypeString(Deputy->GetDeputyType(), "_");
	if (PlayMontage(ActionString))
	{
		DelStamina(Deputy->DelStamina);
	}
}


// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	//WeaponTable = LoadObject<UDataTable>(NULL, TEXT("DataTable'/Game/Main/Data/Weapon_DataTable.Weapon_DataTable'"));

	SetMovement(2, 2);
	LoadEquip();
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
 
void ABaseCharacter::Turn(float Amount)
{
	Super::Turn(Amount);
	AddControllerYawInput(Amount);
}

void ABaseCharacter::LookUp(float Amount)
{
	Super::LookUp(Amount);
	AddControllerPitchInput(Amount);
}


void ABaseCharacter::CreateFootStepBoxs()
{
	LeftFootBox = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftFootBox"));
	RightFootBox = CreateDefaultSubobject<UBoxComponent>(TEXT("RightFootBox"));
	LeftFootBox->SetupAttachment(GetMesh(), FName("LeftToeBase"));
	RightFootBox->SetupAttachment(GetMesh(), FName("RightToeBase"));
	LeftFootBox->OnComponentBeginOverlap.AddDynamic(this, &ABaseCharacter::PlayFootStepSound);
	RightFootBox->OnComponentBeginOverlap.AddDynamic(this, &ABaseCharacter::PlayFootStepSound);
}

void ABaseCharacter::PlayFootStepSound(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UWorld* World = GetWorld();
	if (World->GetTimerManager().IsTimerActive(StepSoundTimer)) return;

	if (OtherActor && Cast<ABaseGround>(OtherActor))
	{
		ABaseGround* BaseGround = Cast<ABaseGround>(OtherActor);

		UDataTable* CharacterGroundSoundsTable = LoadObject<UDataTable>(NULL, TEXT("DataTable'/Game/Main/Data/CharacterGroundSound_DT.CharacterGroundSound_DT'"));
		//UE_LOG(LogTemp, Warning, TEXT("Table->GetName: %s"), *BaseGround->GroundTypeToTableRowname());

		FName RowName = FName(*BaseGround->GroundTypeToTableRowname());
		FString ContextString;
		FBaseGroundSound* Row = CharacterGroundSoundsTable->FindRow<FBaseGroundSound>(RowName, ContextString);
		if (Row)
		{
			//UE_LOG(LogTemp, Warning, TEXT("SoundPath: %s"), *Row->SoundPath);
			USoundBase* GrassStep = LoadObject<USoundBase>(NULL, *Row->SoundPath);
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), GrassStep, BaseGround->GetActorLocation());
			if (World) {
				if (!World->GetTimerManager().IsTimerActive(StepSoundTimer)) {
					World->GetTimerManager().SetTimer(StepSoundTimer, 0.2f, false);
				}
			}
		}
	}
}





FString ABaseCharacter::GetAbilityFilePrefix()
{
	TMap<ECharacterAbility, FString> AbilityTypeToFilename;
	AbilityTypeToFilename.Add(ECharacterAbility::E_SWORD, "Sword");
	AbilityTypeToFilename.Add(ECharacterAbility::E_BOW, "Bow");
	AbilityTypeToFilename.Add(ECharacterAbility::E_MAGIC, "Magic");

	return AbilityTypeToFilename[AbilityType];
}

FString ABaseCharacter::GetActionTypeString(EActionType ActionType)
{
	TMap<EActionType, FString> ActionTypeToString;
	ActionTypeToString.Add(EActionType::E_N1Attack, "N1Attack");
	ActionTypeToString.Add(EActionType::E_N2Attack, "N2Attack");
	ActionTypeToString.Add(EActionType::E_StopTurn180, "StopTurn180");

	return ActionTypeToString[ActionType];
}



float ABaseCharacter::AcceptDamage(float Damage, float Penetrate)
{
	float TrueDamage = Super::AcceptDamage(Damage, Penetrate);
	if (TrueDamage/MaxHealth > 0.05) {
		Stiff(Math::Min(TrueDamage / MaxHealth / 0.05, 2));
	}
	return TrueDamage;
}



void ABaseCharacter::Dead()
{
	Super::Dead();
	UE_LOG(LogTemp, Warning, TEXT("ABaseCharacter::Dead"));
}

void ABaseCharacter::Stiff(float StiffMulti)
{
	BP_Stiff();
	UE_LOG(LogTemp, Warning, TEXT("ABaseCharacter::Stiff"));
	PlayMontage("Stiff");
}

//void ABaseCharacter::Communicate()
//{
//	ABaseInteractive* Interactive = Cast<ABaseInteractive>(CommunicateActor);
//	if (Interactive)  // 可交互物体
//	{
//		Interactive->Communicate();
//	}
//	ABaseNpc* Npc = Cast<ABaseNpc>(CommunicateActor);
//	if (Npc)
//	{
//		Npc->Communicate();
//	}
//}

void ABaseCharacter::SetCommunicateActor(AActor *Actor)
{
	CommunicateActor = Actor;
}

bool ABaseCharacter::IsStopTurn()
{

	return false;
	//FVector ForwardVector = Math::GetForwardVector(FRotator(0, GetActorRotation().Yaw, 0));
	//FVector Velocity = GetVelocity();
	//UE_LOG(LogTemp, Warning, TEXT("IsStopTurn VelocitySize: %f"), Velocity.Size());

	//return (Velocity.X / ForwardVector.X < 0) && (Velocity.Y / ForwardVector.Y < 0) && Velocity.Size()>300;
}

//bool ABaseCharacter::PlayMontage(FString Rowname, FName SectionName)
//{
//	//if(!CanPlayMontage) return false; 
//	UE_LOG(LogTemp, Warning, TEXT("Rowname: %s"), *Rowname);
//	if (IsDead()) return false;
//	if (Rowname == "")	return false;
//
//	FCharacterMontage* CharacterMontage = CharacterMontageDataTable->FindRow<FCharacterMontage>(*Rowname, TEXT("Montage"));
//	if (CharacterMontage && CharacterMontage->Montage) {
//		int32 Num = CharacterMontage->Montage->CompositeSections.Num();
//
//		if (Num > 0)
//		{
//			if (ComboIndex >= Num)
//			{
//				ComboIndex = 0;
//			}
//			if (SectionName == FName())
//			{
//				SectionName = CharacterMontage->Montage->CompositeSections[ComboIndex].SectionName;
//			}
//			PlayAnimMontage(CharacterMontage->Montage, Weapon->Ability.PlayRate, SectionName);
//			ComboIndex++;
//		}
//	}
//	return true;
//}


