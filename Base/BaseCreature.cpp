// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCreature.h"
#include "BaseAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "../ActorComponents/CreatureTracer.h"
using Math = UKismetMathLibrary;



// Sets default values
ABaseCreature::ABaseCreature()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 400;
	SpringArm->CameraLagSpeed = 5;
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm);
	Camera->SetRelativeLocation(FVector(80, 0, 80));

	CreatureTracer = CreateDefaultSubobject<UCreatureTracer>("CreatureTracer");

}



void ABaseCreature::SetMovement(float SpeedMulti, float RotationRataZMulti, EMovementMode Mode)
{
	UE_LOG(LogTemp, Warning, TEXT("ABaseCreature::SetMovement"));
	float Speed = BaseSpeed * SpeedMulti;
	float RotationRateZ = BaseRotationRateZ * RotationRataZMulti;
	GetCharacterMovement()->RotationRate.Yaw = RotationRateZ;

	if (Mode == EMovementMode::MOVE_None)
	{
		Mode = GetCharacterMovement()->MovementMode;
	}

	switch (Mode)
	{
		case EMovementMode::MOVE_Walking:
		{
			UE_LOG(LogTemp, Warning, TEXT("ABaseCreature::SetMovement Walking"));
			GetCharacterMovement()->MaxWalkSpeed = Speed;
			GetCharacterMovement()->SetMovementMode(Mode);
			break;
		}
		case EMovementMode::MOVE_Flying:
		{
			UE_LOG(LogTemp, Warning, TEXT("ABaseCreature::SetMovement Flying"));
			GetCharacterMovement()->MaxFlySpeed = Speed;
			GetCharacterMovement()->SetMovementMode(Mode);
			break;
		}
		// 还有其他模式...
	}

}

void ABaseCreature::SetDilation(float WorldScale, float SelfScale, float Time)
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), WorldScale);
	CustomTimeDilation = SelfScale;

	if (Time != 0)
	{
		UWorld* World = GetWorld();
		if (World) {

			World->GetTimerManager().ClearTimer(DilationTimer);
			World->GetTimerManager().SetTimer(DilationTimer, this, &ABaseCreature::InitDilation, Time);
		}

	}
}

void ABaseCreature::InitDilation()
{
	SetDilation(1, 1);
}



// Called when the game starts or when spawned
void ABaseCreature::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseCreature::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(IsDead()) return;
	DeltaSeconds = DeltaTime;
	Regen();
	Falling();

}

void ABaseCreature::Falling()
{
	if (IsClimbing())
	{
		FallingTime = 0;
	}
	if (IsFalling())
	{
		FallingTime += DeltaSeconds;
	}
	else
	{
		if (!IsClimbing())
		{
			if (FallingTime > 1.0f)
			{
				UE_LOG(LogTemp, Warning, TEXT("HighFall"));
				PlayMontage("HighFall");
				FallingTime = 0;
			}
		}
	}
}

// Called to bind functionality to input
void ABaseCreature::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ABaseCreature::JumpPress);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ABaseCreature::StopJumping);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ABaseCreature::Sprinting);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ABaseCreature::StopSprinting);


	PlayerInputComponent->BindAction("N1Attack", IE_Pressed, this, &ABaseCreature::N1Attack);
	PlayerInputComponent->BindAction("N2Attack", IE_Pressed, this, &ABaseCreature::N2Attack);
	PlayerInputComponent->BindAction("UseDeputy", IE_Pressed, this, &ABaseCreature::UseDeputy);

	PlayerInputComponent->BindAction("Communicate", IE_Pressed, this, &ABaseCreature::Communicate);

	PlayerInputComponent->BindAction("Lock", IE_Pressed, this, &ABaseCreature::Lock);
	PlayerInputComponent->BindAction("Dodge", IE_Pressed, this, &ABaseCreature::Dodge);

	PlayerInputComponent->BindAxis("MoveForward", this, &ABaseCreature::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABaseCreature::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &ABaseCreature::Turn);
	PlayerInputComponent->BindAxis("Lookup", this, &ABaseCreature::LookUp);
}




EDistance ABaseCreature::GetDistanceTypeToTarget()
{
	if (!IsValid(GetTarget()))
	{
		return EDistance::E_FLAT_SFAR;
	}


	FVector DistVector = GetTarget()->GetActorLocation() - GetActorLocation();
	float DistXY = UKismetMathLibrary::VSizeXY(DistVector); // 计算与角色的水平距离
	float DistZ = DistVector.Z; //计算与角色的垂直距离
	if (DistZ < 0)
	{
		DistZ = -DistZ;
	}
	//UE_LOG(LogTemp, Warning, TEXT("DistZ: %f"), DistZ);
	//UE_LOG(LogTemp, Warning, TEXT("DistXY: %f"), DistXY);

	if (IsGround())
	{
		if (0 <= DistXY && DistXY < 500)
		{
			return EDistance::E_FLAT_NEAR;
		}
		else if (500 <= DistXY && DistXY < 1500)
		{
			return EDistance::E_FLAT_MID;
		}
		else if (1500 <= DistXY && DistXY < 2400)
		{
			return EDistance::E_FLAT_FAR;
		}
		else if (2400 <= DistXY)
		{
			return EDistance::E_FLAT_SFAR;
		}
	}
	else
	{
		if (0 <= DistZ && DistZ < 500)
		{
			return EDistance::E_PLUMB_NEAR;
		}
		else if (500 <= DistZ && DistZ < 1000)
		{
			return EDistance::E_PLUMB_MID;
		}
		else if (1000 <= DistZ && DistZ < 1500)
		{
			return EDistance::E_PLUMB_FAR;
		}
		else if (1500 <= DistZ)
		{
			return EDistance::E_PLUMB_SFAR;
		}
	}

	return EDistance::E_FLAT_NEAR;
}





bool ABaseCreature::IsDead()
{
	return CurHealth <= 0;
}

void ABaseCreature::Dead()
{
	UE_LOG(LogTemp, Warning, TEXT("ABaseCreature::Dead"));
	CurHealth = 0;
	if (UGameplayStatics::GetPlayerCharacter(GetWorld(), 0) == this)
	{
		DisableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));  // 暂时这样，这个0还有待商议
	}
	PlayMontage("Dead");
	BP_Dead();
}

void ABaseCreature::Revive()
{
	CurHealth = MaxHealth;
	if (UGameplayStatics::GetPlayerCharacter(GetWorld(), 0) == this)
	{
		EnableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));  // 暂时这样，这个0还有待商议
	}
	GetMesh()->GetAnimInstance()->Montage_Resume(nullptr);
	
}



void ABaseCreature::Regen()
{
	CurHealth += HealthRegen * DeltaSeconds;
	CurHealth = Math::FMin(CurHealth, MaxHealth);

	if (IsSprinting)
	{
		float DelStamina = 0.2 * StaminaRegen * DeltaSeconds;
		if (CurStamina >= DelStamina)
		{
			CurStamina -= DelStamina;
		}
		else
		{
			IsSprinting = false;
			CurStamina = 0;
		}
	}
	else
	{
		CurStamina += StaminaRegen * DeltaSeconds;
	}
	CurStamina = Math::FMin(CurStamina, MaxStamina);
}

float ABaseCreature::AcceptDamage(float Damage, float Penetrate)
{
	UE_LOG(LogTemp, Warning, TEXT("ABaseCreature::AcceptDamage Damage:%f"), Damage);

	if (Damage < 0)
	{
		CurHealth -= Damage;
		CurHealth = UKismetMathLibrary::Min(CurHealth, MaxHealth);
		return 0;
	}


	if (IsInvincible() || IsDead()) return 0;
	float TrueDefense = UKismetMathLibrary::Max(Defense - Penetrate, 0);
	Damage = Damage * 100 / (TrueDefense + 100);

	SetInvincible(0.1f);
	if (Damage > CurHealth)
	{
		Dead();
	}
	else
	{
		CurHealth -= Damage;
	}
	BP_AcceptDamage();
	return Damage;
}




bool ABaseCreature::IsFalling()
{
	return GetCharacterMovement()->IsFalling();
}

bool ABaseCreature::IsFlying()
{
	return GetCharacterMovement()->IsFlying();
}

bool ABaseCreature::IsGround()
{
	return GetCharacterMovement()->IsMovingOnGround();
}

bool ABaseCreature::IsSwimming()
{
	return GetCharacterMovement()->IsSwimming();
}

bool ABaseCreature::IsClimbing()
{
	return Cast<UBaseAnimInstance>(GetMesh()->GetAnimInstance())->IsClimbing;
}

void ABaseCreature::SetClimbing(bool b)
{
	UBaseAnimInstance* AnimInstance = Cast<UBaseAnimInstance>(GetMesh()->GetAnimInstance());
	if (IsValid(AnimInstance))
	{
		AnimInstance->IsClimbing = b;
		if (b)
		{
			UE_LOG(LogTemp, Warning, TEXT("ABaseCreature::SetClimbing T"));
			SetMovement(0.8, 0, EMovementMode::MOVE_Flying);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("ABaseCreature::SetClimbing F"));
			SetMovement(2, 2, EMovementMode::MOVE_Walking);
		}
	}
}


FString ABaseCreature::GetMovementModeString(FString Prefix)
{
	if (IsClimbing())
	{
		return Prefix + "Climbing";
	}
	else if(IsFlying()||IsFalling()||IsSwimming())
	{
		return Prefix + "Air";
	}
	return "";
}

FString ABaseCreature::GetSubMovementModeString(FString Prefix)
{
	if (IsSprinting)
	{
		return Prefix + "Sprint";
	}
	return "";
}

void ABaseCreature::JumpPress()
{
		BP_JumpPress();
		Jump();
		FallingTime = 0;
}

void ABaseCreature::Sprinting()
{
	UE_LOG(LogTemp, Warning, TEXT("Sprinting"));
	IsSprinting = true;
	if(IsGround())
	{
		SetMovement(3, 1.5);
	}
}

void ABaseCreature::StopSprinting()
{
	UE_LOG(LogTemp, Warning, TEXT("StopSprinting"));
	IsSprinting = false;
	if (IsGround())
	{
		SetMovement(2, 1.5);
	}
}

void ABaseCreature::N1Attack()
{
}

void ABaseCreature::N2Attack()
{
}

void ABaseCreature::UseDeputy()
{
}

void ABaseCreature::Lock()
{
	IsLocking = !IsLocking;
}

void ABaseCreature::Dodge()
{
	BP_Dodge();
	UBaseAnimInstance* AnimInstance = Cast<UBaseAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance)
	{
		if (IsClimbing())
		{
			PlayMontage("Dodge", "Climbing");
		}
		else if (AnimInstance->RightSpeed < -30)
		{
			PlayMontage("Dodge", "Left");
		}
		else if (AnimInstance->RightSpeed > 30)
		{
			PlayMontage("Dodge", "Right");
		}
		else if (AnimInstance->ForwardSpeed < -30)
		{
			PlayMontage("Dodge", "Backward");
		}
		else
		{
			PlayMontage("Dodge", "Forward");
		}
	}
}

void ABaseCreature::Communicate()
{
	BP_Communicate();
}

void ABaseCreature::MoveForward(float Amount)
{

	if (IsClimbing())
	{
		MoveUp(Amount);
	}
	else
	{
		if (Amount != 0)
		{
			BP_MoveForward(Amount);
		}
		FRotator R = Math::MakeRotator(0, 0, GetControlRotation().Yaw);
		FVector V = Math::GetForwardVector(R);
		AddMovementInput(V, Amount, false);
	}
}

void ABaseCreature::MoveRight(float Amount)
{
	if (Amount != 0)
	{
		BP_MoveRight(Amount);
	}
	if (IsClimbing())
	{
		FVector V = GetActorRightVector();
		AddMovementInput(V, Amount, false);
	}
	else
	{
		FRotator R = Math::MakeRotator(0, 0, GetControlRotation().Yaw);
		FVector V = Math::GetRightVector(R);
		AddMovementInput(V, Amount, false);
	}
}

// ClimbUp /  FlyUp
void ABaseCreature::MoveUp(float Amount)
{
	BP_MoveUp(Amount);
	AddMovementInput(FVector(0,0,1), Amount, false);
}

void ABaseCreature::Turn(float Amount)
{
	AddControllerYawInput(Amount);
}

void ABaseCreature::LookUp(float Amount)
{
	AddControllerPitchInput(Amount);
}

void ABaseCreature::MontageStart(UAnimMontage* Montage)
{
	UE_LOG(LogTemp, Warning, TEXT("MontageStart"));
	CanPlayMontage = false;

	BP_MontageStart(Montage);
}

void ABaseCreature::MontageEnd(UAnimMontage* Montage, bool bInterrupted)
{
	UE_LOG(LogTemp, Warning, TEXT("MontageEnd"));
	CanPlayMontage = true;
	BP_MontageEnd(Montage, bInterrupted);
}


float ABaseCreature::GetHealthPercent()
{
	return CurHealth / MaxHealth;
}

float ABaseCreature::GetStaminaPercent()
{
	return CurStamina / MaxStamina;
}


void ABaseCreature::SetInvincible(float Time)
{
	UWorld* World = GetWorld();
	if (World) {
		if (!World->GetTimerManager().IsTimerActive(InvincibleTimer)) {
			World->GetTimerManager().SetTimer(InvincibleTimer, Time, false);
		}
	}
}
bool ABaseCreature::IsInvincible()
{
	UWorld* World = GetWorld();
	return World && World->GetTimerManager().IsTimerActive(InvincibleTimer);
}

void ABaseCreature::SetTarget(ABaseCreature* C)
{
	Target = C;
}
ABaseCreature* ABaseCreature::GetTarget()
{
	return Target;
}



bool ABaseCreature::PlayMontage(FString Rowname, FString SectionName, float PlayRate)
{
	//if(!CanPlayMontage) return false; 
	UE_LOG(LogTemp, Warning, TEXT("ABaseCreature::PlayMontage Rowname: %s"), *Rowname);
	if (Rowname == "")	return false;
	if (Rowname!="Dead" && IsDead()) return false;
	if (!IsValid(CreatureMontageDataTable)) return false;

	FCreatureMontage* CreatureMontage = CreatureMontageDataTable->FindRow<FCreatureMontage>(*Rowname, TEXT("Montage"));
	if (CreatureMontage && CreatureMontage->Montage) {
		int32 Num = CreatureMontage->Montage->CompositeSections.Num();

		if (Num > 0)
		{
			if (ComboIndex >= Num)
			{
				ComboIndex = 0;
			}
			if (SectionName == "")
			{
				SectionName = CreatureMontage->Montage->CompositeSections[ComboIndex].SectionName.ToString();
			}
			PlayAnimMontage(CreatureMontage->Montage, PlayRate, *SectionName);
			ComboIndex++;
		}
	}
	return true;
}