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

#include "Kismet/KismetMathLibrary.h"
using Math = UKismetMathLibrary;





// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetCapsuleComponent()->SetCapsuleSize(42,96,true);
	GetCharacterMovement()->bOrientRotationToMovement = true;
	this->bUseControllerRotationYaw = false;
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 400.f;	
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm);
	this->CreateFootStepBoxs();
	this->CalBaseAbility();

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
}

void ABaseCharacter::LoadWeapon()
{

	if (CurWeaponID != "0")
	{
		UDataTable* WeaponTable = LoadObject<UDataTable>(NULL, TEXT("/Game/Main/Data/Weapon_DataTable"));
		UE_LOG(LogTemp, Warning, TEXT("Table->GetName: %s"), *WeaponTable->GetName());

		FName RowName = FName(*CurWeaponID);
		FString ContextString;
		FWeapon* Row = WeaponTable->FindRow<FWeapon>(RowName, ContextString);

		//UE_LOG(LogTemp, Warning, TEXT("Row: %s, %s, %s"), *Row->WeaponName, *Row->WeaponDescription, *Row->WeaponActorPath);
		if (Row)
		{
			TSubclassOf<ABaseWeapon> SubWeaponClass = LoadClass<ABaseWeapon>(nullptr, *Row->WeaponActorPath);
			UE_LOG(LogTemp, Warning, TEXT("Weapon->GetName11"));
			if (SubWeaponClass != nullptr)
			{
				Weapon = GetWorld()->SpawnActor<ABaseWeapon>(SubWeaponClass);
				//Weapon->SetActorEnableCollision(false);
				if (Weapon)
				{
					FAttachmentTransformRules Rules = FAttachmentTransformRules(EAttachmentRule::KeepRelative, false);
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
		CurStamina += StaminaRegen * DeltaTime;
		CurStamina = Math::FMin(CurStamina, MaxStamina);
	}

}

void ABaseCharacter::BaseAttack()
{
	if (!IsValid(Weapon))
	{
		return;
	}
	EWeaponType CurWeaponType = Weapon->GetWeaponType();
	Weapon->WeaponAttack();
	switch (CurWeaponType)
	{
		case EWeaponType::E_SWORD:
		{
			//GetMesh()->PlayAnimation();
		}
		default:
			break;
	} 
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
	this->ControlMaxWalkSpeed();
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

	PlayerInputComponent->BindAction("LeftMouse", IE_Pressed, this, &ABaseCharacter::BaseAttack);

	PlayerInputComponent->BindAxis("MoveForward", this, &ABaseCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABaseCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &ABaseCharacter::Turn);
	PlayerInputComponent->BindAxis("Lookup", this, &ABaseCharacter::Lookup);
}


void ABaseCharacter::ControlMaxWalkSpeed()
{
	float TempSpeed = 600;
	if (IsSprinting)
	{	
		TempSpeed = 900;
	}
	GetCharacterMovement()->MaxWalkSpeed = TempSpeed;
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
	////UE_LOG(LogTemp, Warning, TEXT("Yaw: %f"), GetControlRotation().Yaw);
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
	_LeftFootBox = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftFootBox"));
	_RightFootBox = CreateDefaultSubobject<UBoxComponent>(TEXT("RightFootBox"));
	_LeftFootBox->SetupAttachment(GetMesh(), "LeftFoot");
	_RightFootBox->SetupAttachment(GetMesh(), "RightFoot");
	_LeftFootBox->OnComponentBeginOverlap.AddDynamic(this, &ABaseCharacter::PlayFootStepSound);
	_RightFootBox->OnComponentBeginOverlap.AddDynamic(this, &ABaseCharacter::PlayFootStepSound);
}

void ABaseCharacter::PlayFootStepSound(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && Cast<ABaseGround>(OtherActor))
	{
		ABaseGround* BaseGround = Cast<ABaseGround>(OtherActor);

		UDataTable* CharacterGroundSoundsTable = LoadObject<UDataTable>(NULL, TEXT("/Game/Main/Data/CharacterGroundSounds_DataTable"));
		//UE_LOG(LogTemp, Warning, TEXT("Table->GetName: %s"), *Table->GetName());

		FName RowName = FName(*BaseGround->_GroundType);

		FString ContextString;
		FBaseGroundSound* Row = CharacterGroundSoundsTable->FindRow<FBaseGroundSound>(RowName, ContextString);
		if (Row)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Row: %s, %s"), *pRow->GroundDescription, *pRow->SoundPath);
			USoundBase* GrassStep = LoadObject<USoundBase>(NULL, *Row->SoundPath);
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), GrassStep, BaseGround->GetActorLocation());
		}
	}
}
