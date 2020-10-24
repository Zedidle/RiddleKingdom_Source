// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "Components/InputComponent.h"
#include "BaseGameInstance.h"
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
	UBaseGameInstance* GameInstance = Cast<UBaseGameInstance>(GetGameInstance());
	if (!IsValid(GameInstance)) return;
	UDataTable* DT_Weapon = GameInstance->DT_Weapon;

	if (WeaponID == "0" || !IsValid(DT_Weapon)) return;
	UE_LOG(LogTemp, Warning, TEXT("ABaseCharacter::LoadWeapon WeaponID:%s"), *CurWeaponID);

	CurWeaponID = WeaponID;
	FName RowName = FName(*CurWeaponID);
	FString ContextString;
	FWeapon* Row = DT_Weapon->FindRow<FWeapon>(RowName, ContextString);

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
	UBaseGameInstance* GameInstance = Cast<UBaseGameInstance>(GetGameInstance());
	if (!IsValid(GameInstance)) return;
	UDataTable* DT_Deputy = GameInstance->DT_Weapon;

	if (DeputyID == "0" || !IsValid(DT_Deputy)) return;
	UE_LOG(LogTemp, Warning, TEXT("ABaseCharacter::LoadDeputy DeputyID:%s"), *CurDeputyID);
	CurDeputyID = DeputyID;
	FName RowName = FName(*CurDeputyID);
	FString ContextString;
	FDeputy* Row = DT_Deputy->FindRow<FDeputy>(RowName, ContextString);

	if(Row)
	{
		UE_LOG(LogTemp, Warning, TEXT("ABaseCharacter::LoadDeputy 111"));
		TSubclassOf<ABaseDeputy> SubDeputyClass = LoadClass<ABaseDeputy>(nullptr, *Row->DeputyActorPath);
		if (SubDeputyClass != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("ABaseCharacter::LoadDeputy 222"));

			if (Deputy) {
				Deputy->Destroy();
			}
			Deputy = GetWorld()->SpawnActor<ABaseDeputy>(SubDeputyClass);
			if (Deputy)
			{
				UE_LOG(LogTemp, Warning, TEXT("ABaseCharacter::LoadDeputy 333"));
				FAttachmentTransformRules Rules = FAttachmentTransformRules(EAttachmentRule::KeepRelative, false);
				Deputy->User = this;
				Deputy->AttachToComponent(GetMesh(), Rules, "DeputyHold");
			}
		}
	}
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
	if (Enable)
	{
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
}


void ABaseCharacter::OnDeputyUse(bool Enable)
{
	UE_LOG(LogTemp, Warning, TEXT("ABaseCharacter::OnDeputyUse"));

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
}

void ABaseCharacter::OnDeputyDamageEnableChanged(bool Enable)
{
	UE_LOG(LogTemp, Warning, TEXT("ABaseCharacter::OnDeputyDamageEnableChanged"));
}

void ABaseCharacter::OnDeputyComboEnableChanged(bool Enable)
{
	UE_LOG(LogTemp, Warning, TEXT("ABaseCharacter::OnDeputyComboEnableChanged"));
}

void ABaseCharacter::ComboTimeout()
{
}

void ABaseCharacter::DeputyComboTimeout()
{
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
	if (!IsValid(Weapon))
	{
		PlayMontage("N1Attack");
		return;
	}
	if (Weapon->DelStamina > CurStamina) return;
	FString ActionString = "N1Attack" + GetMovementModeString("_") + GetSubMovementModeString("_") + 
		Util::GetWeaponTypeString(Weapon->WeaponType,"_");

	UE_LOG(LogTemp, Warning, TEXT("ABaseCharacter::N1Attack: %s"), *ActionString);

	if (PlayMontage(ActionString))
	{
		DelStamina(Weapon->DelStamina);
	}
}

void ABaseCharacter::N2Attack()
{
	if (!IsValid(Weapon))
	{
		PlayMontage("N2Attack");
		return;
	}
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


float ABaseCharacter::AcceptDamage(float Damage, float Penetrate)
{
	float TrueDamage = Super::AcceptDamage(Damage, Penetrate);
	if (TrueDamage/MaxHealth > StiffPercent) {
		Stiff(Math::Min(TrueDamage / MaxHealth / StiffPercent, 2));
	}
	return TrueDamage;
}


void ABaseCharacter::Dead(bool bClearHealth)
{
	Super::Dead(bClearHealth);
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

bool ABaseCharacter::AbilityCanUseWeaponType(EWeaponType WeaponType)
{
	return Util::AbilityCanUseWeaponType(AbilityType, WeaponType);
}

