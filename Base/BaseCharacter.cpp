// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "Components/InputComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/EngineTypes.h"
#include "Components/SkeletalMeshComponent.h"
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
#include "Kismet/KismetMathLibrary.h"
using Math = UKismetMathLibrary;




// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetCapsuleComponent()->SetCapsuleSize(42,96,true);
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->AirControl = 5;
	this->bUseControllerRotationYaw = false;
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 400.f;	
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm);
	Camera->SetRelativeLocation(FVector(80, 0, 80));


	this->CreateFootStepBoxs();
	this->CalBaseAbility();


	FString Filename = "DataTable'/Game/Main/Characters/" + GetAbilityFilePrefix() + "Character/CharacterMontage_DataTable.CharacterMontage_DataTable'";
	static ConstructorHelpers::FObjectFinder<UDataTable> MontageDataTableObject(*Filename);
	if (MontageDataTableObject.Succeeded()) {
		CharacterMontageDataTable = MontageDataTableObject.Object; // 角色动画蒙太奇集
	}
	


}


void ABaseCharacter::CalBaseAbility()
{
	// 应该还要额外读取存档
	CurWeaponID = "1";
	CurHealth = 100;
	MaxHealth = 100;
	HealthRegen = 1;
	CurStamina = 100;
	MaxStamina = 100;
	StaminaRegen = 10;

	Attributes.Body = 1;
	Attributes.Power = 1;
	Attributes.Sprit = 1;
	Attributes.Agile = 1;
}

void ABaseCharacter::LoadWeapon()
{

	if (CurWeaponID != "0")
	{
		UDataTable* WeaponTable = LoadObject<UDataTable>(NULL, TEXT("DataTable'/Game/Main/Data/Weapon_DataTable.Weapon_DataTable'"));
		UE_LOG(LogTemp, Warning, TEXT("Table->GetName: %s"), *WeaponTable->GetName());
		FName RowName = FName(*CurWeaponID);
		FString ContextString;
		FWeapon* Row = WeaponTable->FindRow<FWeapon>(RowName, ContextString);

		if (Row)
		{
			TSubclassOf<ABaseWeapon> SubWeaponClass = LoadClass<ABaseWeapon>(nullptr, *Row->WeaponActorPath);
			UE_LOG(LogTemp, Warning, TEXT("Weapon->LoadWeapon1"));
			if (SubWeaponClass != nullptr)
			{
				Weapon = GetWorld()->SpawnActor<ABaseWeapon>(SubWeaponClass);
				if (Weapon)
				{	
					FAttachmentTransformRules Rules = FAttachmentTransformRules(EAttachmentRule::KeepRelative, false);
					Weapon->SetCurAbility(Attributes);
					Weapon->AttachToComponent(GetMesh(), Rules, "WeaponHold");
				}
			}
		}
	}
}


void ABaseCharacter::LoadBaseHUD()
{
	if (BaseHUD)
	{
		UUserWidget* UserWidget = CreateWidget<UUserWidget>(GetWorld(), BaseHUD);
		UserWidget->AddToViewport();
	}
}

void ABaseCharacter::Regen(float DeltaTime)
{
	if (Math::InRange_FloatFloat(CurHealth, 0, MaxHealth))
	{
		CurHealth += HealthRegen * DeltaTime;
		CurHealth = Math::FMin(CurHealth, MaxHealth);
	}

	if (Math::InRange_FloatFloat(CurStamina, 0, MaxStamina))
	{
		if (IsSprinting)
		{
			CurStamina -= 0.2 * StaminaRegen * DeltaTime;
		}
		else
		{
			CurStamina += StaminaRegen * DeltaTime;
		}
		CurStamina = Math::FMin(CurStamina, MaxStamina);
	}

	if (CurActionCold > 0)
	{
		CurActionCold -= DeltaTime;
	}

}

bool ABaseCharacter::CanAction()
{
	if (CurActionCold > 0)
	{
		return false;
	}
	else
	{
		return true;
	}
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
		CanAttack = true;
		UWorld* World = GetWorld();
		if (World) {
			World->GetTimerManager().ClearTimer(ComboWaitTimer);
			World->GetTimerManager().SetTimer(ComboWaitTimer, this, &ABaseCharacter::ComboTimeout, ComboWaittime);
		}
	}
}

void ABaseCharacter::OnAttackDamageEnableChanged(bool Enable)
{
	if (Enable)
	{
		if (IsValid(Weapon))
		{
			Weapon->WeaponAttack();
		}
		//CanAttack = false;

	}
	else
	{
		if (IsValid(Weapon))
		{
			Weapon->WeaponStopAttack();
		}
		//CanAttack = true;
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
		CanAttack = false;
	}
}

void ABaseCharacter::ComboTimeout()
{
	if (CanAttack)
	{
		ComboIndex = 0;
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


// ActiveIndex: 处于对应Montage的激活动作下标
void ABaseCharacter::BaseAction(EActionType ActionType, int ActiveIndex)
{
	if (!CanAction()) return;
	if (!CanAttack) return;
	if (ComboIndex > 0 && !CanCombo) return;  // 未到可以连击的状态
	SetCurActionCold(BaseAttackActionCold);
	
	UE_LOG(LogTemp, Warning, TEXT("CharacterMontage ComboIndex: %d"), ComboIndex);

	ActionActiveIndex = ActiveIndex;
	FString ActionTypeString = GetActionTypeString(ActionType);
	FName RowName = *(GetWeaponMontagePrefix() + "_" + ActionTypeString);
	FCharacterMontage* CharacterMontage = CharacterMontageDataTable->FindRow<FCharacterMontage>(RowName, TEXT("Montage"));
	if (CharacterMontage && CharacterMontage->Montage) {
		int32 Num = CharacterMontage->Montage->CompositeSections.Num();
		//UE_LOG(LogTemp, Warning, TEXT("CharacterMontage: %d"), Num);

		if (Num > 0 && ComboIndex < Num) {

			FString SectionName = CharacterMontage->Montage->CompositeSections[ComboIndex].SectionName.ToString();

			//UE_LOG(LogTemp, Warning, TEXT("SectionName: %s"), *SectionName);
			PlayAnimMontage(CharacterMontage->Montage, 1.f, CharacterMontage->Montage->CompositeSections[ComboIndex].SectionName);

			DelStamina(Weapon->NormalAttack_DelStamina);
			ComboIndex++;
			CanAttack = false;
			if (ComboIndex == Num)
			{
				ComboIndex = 0;
				CanCombo = false;
			}
		}
	}
}

void ABaseCharacter::NormalAttack()
{	
	BaseAction(EActionType::E_NormalAttack, 1);
}

void ABaseCharacter::N2Attack()
{
	BaseAction(EActionType::E_N2Attack, 2);
}



// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	this->LoadBaseHUD();
	this->LoadWeapon();
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	float FrameRate = 1 / DeltaTime;
	this->ControlMovement();
	this->Regen(DeltaTime);

}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ABaseCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ABaseCharacter::StopJumping);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ABaseCharacter::Sprinting);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ABaseCharacter::StopSprinting);


	PlayerInputComponent->BindAction("LeftMouse", IE_Pressed, this, &ABaseCharacter::NormalAttack);
	PlayerInputComponent->BindAction("RightMouse", IE_Pressed, this, &ABaseCharacter::N2Attack);



	PlayerInputComponent->BindAxis("MoveForward", this, &ABaseCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABaseCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &ABaseCharacter::Turn);
	PlayerInputComponent->BindAxis("Lookup", this, &ABaseCharacter::Lookup);
}

void ABaseCharacter::ControlMovement()
{
	float TempSpeed = 600;
	float RotationRate = 360;
	if (IsSprinting)
	{
		TempSpeed = TempSpeed * 1.5;
		RotationRate = RotationRate * 0.6;
	}
	if (!CanAttack)
	{
		TempSpeed = TempSpeed * 0.8;
		RotationRate = RotationRate * 0.8;
	}
	if (!CanAction())
	{
		TempSpeed = TempSpeed * 0.5;
		RotationRate = RotationRate * 0.7;
	}
	GetCharacterMovement()->MaxWalkSpeed = TempSpeed;
	GetCharacterMovement()->RotationRate.Yaw = RotationRate;
}

void ABaseCharacter::SetCurBoss(ABaseMonster* Monster)
{
	CurBoss = Monster;
}

ABaseMonster* ABaseCharacter::GetCurBoss()
{
	return CurBoss;
}

void ABaseCharacter::Sprinting()
{
	IsSprinting = true;
}

void ABaseCharacter::StopSprinting()
{
	IsSprinting = false;
}

void ABaseCharacter::MoveForward(float Amount)
{
	FRotator Rotator = Math::MakeRotator(0,0, GetControlRotation().Yaw);
	FVector ForwardVector = Math::GetForwardVector(Rotator);
	this->AddMovementInput(ForwardVector, Amount, false);
}

void ABaseCharacter::MoveRight(float Amount)
{
	FRotator Rotator = Math::MakeRotator(0, 0, GetControlRotation().Yaw);
	FVector RightVector = Math::GetRightVector(Rotator);
	this->AddMovementInput(RightVector, Amount, false);
}
 
void ABaseCharacter::Turn(float Amount)
{
	this->AddControllerYawInput(Amount);
}

void ABaseCharacter::Lookup(float Amount)
{
	this->AddControllerPitchInput(Amount);
}


void ABaseCharacter::CreateFootStepBoxs()
{
	LeftFootBox = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftFootBox"));
	_RightFootBox = CreateDefaultSubobject<UBoxComponent>(TEXT("RightFootBox"));
	LeftFootBox->SetupAttachment(GetMesh(), FName("LeftToeBase"));
	_RightFootBox->SetupAttachment(GetMesh(), FName("RightToeBase"));
	LeftFootBox->OnComponentBeginOverlap.AddDynamic(this, &ABaseCharacter::PlayFootStepSound);
	_RightFootBox->OnComponentBeginOverlap.AddDynamic(this, &ABaseCharacter::PlayFootStepSound);
}

void ABaseCharacter::PlayFootStepSound(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UWorld* World = GetWorld();
	if (World->GetTimerManager().IsTimerActive(StepSoundTimer)) return;

	if (OtherActor && Cast<ABaseGround>(OtherActor))
	{
		ABaseGround* BaseGround = Cast<ABaseGround>(OtherActor);

		UDataTable* CharacterGroundSoundsTable = LoadObject<UDataTable>(NULL, TEXT("/Game/Main/Data/CharacterGroundSound_DataTable"));
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

FString ABaseCharacter::GetWeaponMontagePrefix()
{
	TMap<EWeaponType, FString> WeaponTypeToMontagePrefix;
	WeaponTypeToMontagePrefix.Add(EWeaponType::E_NONE, "");
	WeaponTypeToMontagePrefix.Add(EWeaponType::E_GREATSWORD, "GreatSword");
	WeaponTypeToMontagePrefix.Add(EWeaponType::E_SWORD, "Sword");
	WeaponTypeToMontagePrefix.Add(EWeaponType::E_BOW, "Bow");
	WeaponTypeToMontagePrefix.Add(EWeaponType::E_MAGIC, "Magic");

	return WeaponTypeToMontagePrefix[Weapon->GetWeaponType()];

}

FString ABaseCharacter::GetActionTypeString(EActionType ActionType)
{
	TMap<EActionType, FString> ActionTypeToString;
	ActionTypeToString.Add(EActionType::E_NormalAttack, "NormalAttack");
	ActionTypeToString.Add(EActionType::E_N2Attack, "N2Attack");
	ActionTypeToString.Add(EActionType::E_Jump, "Jump");

	return ActionTypeToString[ActionType];
}



void ABaseCharacter::AcceptDamage(float Damage, float Penetrate)
{
	if(IsInvincible()) return;
	if(IsDead()) return;
	UE_LOG(LogTemp, Warning, TEXT("ABaseCharacter::TakeDamage Damage: %f"), Damage);
	float TrueDefense = UKismetMathLibrary::Max(Defense - Penetrate, 0);
	float TrueDamage = Damage * 100 / (TrueDefense + 100);
	
	SetInvincible(0.1f);
	if (TrueDamage > CurHealth)
	{
		CurHealth = 0;
		Dead();
	}
	else
	{	
		CurHealth -= TrueDamage;
	}
	BP_AcceptDamage();
}



void ABaseCharacter::SetInvincible(float Time)
{
	UWorld* World = GetWorld();
	if (World) {
		if (!World->GetTimerManager().IsTimerActive(InvincibleTimer)) {
			World->GetTimerManager().SetTimer(InvincibleTimer, Time, false);
		}
	}
}

bool ABaseCharacter::IsInvincible()
{
	UWorld* World = GetWorld();
	return World && World->GetTimerManager().IsTimerActive(InvincibleTimer);
}

bool ABaseCharacter::IsDead()
{
	return CurHealth <= 0;
}

void ABaseCharacter::Dead()
{
	BP_Dead();
}
