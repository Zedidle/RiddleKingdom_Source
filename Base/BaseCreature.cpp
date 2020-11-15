// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCreature.h"
#include "BaseAnimInstance.h"
#include "BaseGameInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "../ActorComponents/CreatureWidgetComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "../Util.h"
#include "../ActorComponents/CreatureTracer.h"
using Math = UKismetMathLibrary;



// Sets default values
ABaseCreature::ABaseCreature()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationYaw = false;

	HealthWidget = CreateDefaultSubobject<UCreatureWidgetComponent>("HealthWidget");
	HealthWidget->SetupAttachment(RootComponent);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraRotationLagSpeed = 10;
	SpringArm->CameraLagSpeed = 5;
	SpringArm->bUsePawnControlRotation = true;
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm);
	Camera->SetRelativeLocation(FVector(80, 90, 80));

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(90, 90, 120);
	CreatureTracer = CreateDefaultSubobject<UCreatureTracer>("CreatureTracer");

}


void ABaseCreature::AddSkillOnUsing(ABaseSkill* Skill)
{
	if (IsValid(Skill))
	{
		SkillsOnUsing.AddUnique(Skill);
	}
}

void ABaseCreature::ActionModes()
{
	if (IsPlayerControlled()) return;
	if (!IsValid(GetTarget())) return;  // 没有看到角色
	if (IsDead()) return;
	ActionInterval -= DeltaSeconds;

	FVector DistVector = GetTarget()->GetActorLocation() - GetActorLocation();
	float DistXY = Math::VSizeXY(DistVector); // 计算与角色的水平距离
	float DistZ = DistVector.Z; //计算与角色的垂直距离

	if (ActionInterval <= 0)
	{
		BP_ActionModes(); // 先采取蓝图的行动，有可能增加ActionInterval
	}

	if (ActionInterval < 0)  // 采取行动
	{
		ActionInterval = 1;
	}
}

void ABaseCreature::SetMovement(float SpeedMulti, float RotationRataZMulti, EMovementMode Mode)
{
	//UE_LOG(LogTemp, Warning, TEXT("ABaseCreature::SetMovement"));
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
			//UE_LOG(LogTemp, Warning, TEXT("ABaseCreature::SetMovement Walking"));
			GetCharacterMovement()->MaxWalkSpeed = Speed;
			GetCharacterMovement()->SetMovementMode(Mode);
			break;
		}
		case EMovementMode::MOVE_Flying:
		{
			//UE_LOG(LogTemp, Warning, TEXT("ABaseCreature::SetMovement Flying"));
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

void ABaseCreature::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	UBaseGameInstance* GameInstance = Cast<UBaseGameInstance>(GetGameInstance());
	if (IsValid(GameInstance))
	{
		if (NewController == UGameplayStatics::GetPlayerController(GetWorld(), 0))
		{
			bBeenControlled = true;
			Faction = EFaction::E_Character;
			GameInstance->AddCreatureUsed(this);
			GameInstance->ShowShootAimHUD(bShowShootAnim);
		}
	}
}



// Called when the game starts or when spawned
void ABaseCreature::BeginPlay()
{
	Super::BeginPlay();
	SpringArm->TargetArmLength = BaseSpringArmLength;
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	PosStartInWorld = GetActorLocation();
	
	UnitDistance = UnitDistance * GetActorScale().X;

	HealthWidget->SetRelativeLocation(FVector(0, 0, 1.2 * GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight()));

	ComboIndex.Add("N1Attack", 0);
	ComboIndex.Add("N2Attack", 0);
	ComboIndex.Add("Jump", 0);
	ComboIndex.Add("Stiff", 0);


}

// Called every frame
void ABaseCreature::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(IsDead()) return;
	DeltaSeconds = DeltaTime;
	Tick_Regen();
	Tick_MoveToTarget();
	Tick_LockToFaceTarget();
	Tick_CalFalling();
}

void ABaseCreature::Tick_Regen()
{
	CurHealth += HealthRegen * DeltaSeconds;
	CurHealth = Math::FMin(CurHealth, MaxHealth);

	if (bSprinting)
	{
		float DelStamina = 0.2 * StaminaRegen * DeltaSeconds;
		if (CurStamina >= DelStamina)
		{
			CurStamina -= DelStamina;
		}
		else
		{
			bSprinting = false;
			CurStamina = 0;
		}
	}
	else
	{
		CurStamina += StaminaRegen * 0.5 * DeltaSeconds;
	}
	CurStamina = Math::FMin(CurStamina, MaxStamina);
}


void ABaseCreature::Tick_LockToFaceTarget()
{
	if (!bLocking || !IsValid(Target) || bSprinting) return;
	if (Target->IsDead())
	{
		bLocking = false;
	}

	FVector SelfLocation = GetActorLocation();
	FVector TargetLocation = Target->GetActorLocation();
	SelfLocation.Z = 0;
	TargetLocation.Z = 0;
	FRotator RotationToTarget = Math::FindLookAtRotation(SelfLocation, TargetLocation);

	if (bFaceToTarget)
	{
		FRotator SelfForwardVectorRotX = Math::MakeRotFromX(GetActorForwardVector());
		SetActorRotation(Math::RLerp(SelfForwardVectorRotX, RotationToTarget, 0.2, true));
	}

	FRotator ControlRotation = GetController()->GetControlRotation();
	FRotator ControlLerpRotator = Math::RLerp(ControlRotation, RotationToTarget, 0.05, true);
	GetController()->SetControlRotation(FRotator(ControlRotation.Pitch, ControlLerpRotator.Yaw, ControlRotation.Roll));
}

void ABaseCreature::Tick_MoveToTarget(float Time, float SpeedMulti)
{
	if (Time > 0)
	{
		Time_MoveToTarget = Time;
		SetMovement(SpeedMulti);
	}
	if (Time_MoveToTarget <= 0) return;
	Time_MoveToTarget -= DeltaSeconds;

	if (IsValid(Target))
	{
		if (GetDistanceTypeToTarget() != EDistance::E_FLAT_SNEAR)
		{
			FVector V = Math::GetForwardVector(Math::FindLookAtRotation(GetActorLocation(), Target->GetActorLocation()));
			AddMovementInput(V, 1, false);
		}
		BP_Tick_MoveToTarget();
	}
}

void ABaseCreature::Tick_CalFalling()
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
	PlayerInputComponent->BindAction("Communicate", IE_Released, this, &ABaseCreature::StopCommunicate);


	PlayerInputComponent->BindAction("Lock", IE_Pressed, this, &ABaseCreature::Lock);
	PlayerInputComponent->BindAction("Dodge", IE_Pressed, this, &ABaseCreature::Dodge);

	PlayerInputComponent->BindAction("FarView", IE_Pressed, this, &ABaseCreature::FarView);
	PlayerInputComponent->BindAction("NearView", IE_Pressed, this, &ABaseCreature::NearView);

	PlayerInputComponent->BindAxis("MoveForward", this, &ABaseCreature::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABaseCreature::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &ABaseCreature::Turn);
	PlayerInputComponent->BindAxis("Lookup", this, &ABaseCreature::LookUp);
}




EDistance ABaseCreature::GetDistanceTypeToTarget()
{
	if (!IsValid(Target))
	{
		return EDistance::E_FLAT_SFAR;
	}


	FVector DistVector = Target->GetActorLocation() - GetActorLocation();
	float DistXY = Math::VSizeXY(DistVector); // 计算与角色的水平距离
	float DistZ = DistVector.Z; //计算与角色的垂直距离
	if (DistZ < 0)
	{
		DistZ = -DistZ;
	}
	//UE_LOG(LogTemp, Warning, TEXT("DistZ: %f"), DistZ);
	//UE_LOG(LogTemp, Warning, TEXT("DistXY: %f"), DistXY);
	float DistXY_Unit = DistXY / UnitDistance;
	float DistZ_Unit = DistZ / UnitDistance;

	if (IsGround())
	{
		if (Target->IsFalling())
		{
			if (0 <= DistZ_Unit && DistZ_Unit < 1.5)
			{
				return EDistance::E_PLUMB_NEAR;
			}
			else if (1.5 <= DistZ_Unit && DistZ_Unit < 3)
			{
				return EDistance::E_PLUMB_MID;
			}
			else if (3 <= DistZ_Unit && DistZ_Unit < 5)
			{
				return EDistance::E_PLUMB_FAR;
			}
			else if (5 <= DistZ_Unit)
			{
				return EDistance::E_PLUMB_SFAR;
			}
		}
		else
		{

			if (0 <= DistXY_Unit && DistXY_Unit < 1)
			{
				return EDistance::E_FLAT_SNEAR;
			}
			else if (1 <= DistXY_Unit && DistXY_Unit < 1.5)
			{
				return EDistance::E_FLAT_NEAR;
			}
			else if (1.5 <= DistXY_Unit && DistXY_Unit < 5)
			{
				return EDistance::E_FLAT_MID;
			}
			else if (5 <= DistXY_Unit && DistXY_Unit < 7)
			{
				return EDistance::E_FLAT_FAR;
			}
			else if (7 <= DistXY_Unit)
			{
				return EDistance::E_FLAT_SFAR;
			}
		}
	}
	else
	{
		if (0 <= DistZ_Unit && DistZ_Unit < 1.5)
		{
			return EDistance::E_PLUMB_NEAR;
		}
		else if (1.5 <= DistZ_Unit && DistZ_Unit < 3)
		{
			return EDistance::E_PLUMB_MID;
		}
		else if (3 <= DistZ_Unit && DistZ_Unit < 5)
		{
			return EDistance::E_PLUMB_FAR;
		}
		else if (5 <= DistZ_Unit)
		{
			return EDistance::E_PLUMB_SFAR;
		}
	}

	return EDistance::E_FLAT_SNEAR;
}





void ABaseCreature::Dead(bool bClearHealth)
{
	// bClearHeal决定是死亡还是脱离玩家控制
	UBaseGameInstance* GameInstance = Cast<UBaseGameInstance>(GetGameInstance());
	if (!IsValid(GameInstance)) return;

	if (bClearHealth)
	{
		CurHealth = 0;
	}

	if (IsPlayerControlled())
	{
		DisableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));  // 暂时这样，这个0还有待商议

		if (!bLevelKey)
		{
			GameInstance->ShowDeadHUD(true);
		}
		else
		{
			GameInstance->ShowKeyDeadHUD(true);
		}
	}


	SetMovement(2, 2, EMovementMode::MOVE_Falling);
	PlayMontage("Dead");
	BP_Dead();
}

void ABaseCreature::Revive()
{
	if(IsTrueDead()) return;
	UE_LOG(LogTemp, Warning, TEXT("ABaseCreature::Revive"));
	if (!bBeenControlled)
	{
		CurHealth = MaxHealth;
		CurStamina = MaxStamina;
	}
	if (IsPlayerControlled())
	{
		EnableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));  // 暂时这样，这个0还有待商议
	}

	bLocking = false;
	bSprinting = false;

	GetMesh()->GetAnimInstance()->Montage_Resume(nullptr);

	SetInvincible(3);
	PlayMontage("Revive");
	BP_Revive();

}






float ABaseCreature::AcceptDamage(float Damage, float Penetrate)
{
	UE_LOG(LogTemp, Warning, TEXT("ABaseCreature::AcceptDamage Damage:%f"), Damage);
	if (Damage < 0)  // 治疗
	{
		CurHealth -= Damage;
		CurHealth = Math::Min(CurHealth, MaxHealth);
		return 0;
	}


	if (IsInvincible() || IsDead()) return 0;
	if( IsTrueDead() ) return 0;
	if (bBeenControlled && !IsPlayerControlled()) return 0;
	// 后面还要考虑侵入盟友后，盟友自己控制的情况


	float TrueDefense = Math::Max(Defense - Penetrate, 0);
	Damage = Damage * 100 / (TrueDefense + 100);

	Damage = Util::CalDamageByDifficulty(Damage, IsPlayerControlled());

	SetInvincible(0.1f);
	if (Damage > CurHealth)
	{
		Dead();
	}
	else
	{
		CurHealth -= Damage;
	}
	BP_AcceptDamage(Damage);
	return Damage;
}




bool ABaseCreature::SameFaction(ABaseCreature* OtherCreature)
{
	if (IsValid(OtherCreature))
	{
		return Faction == OtherCreature->Faction;
	}
	return false;
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
	if (bSprinting)
	{
		return Prefix + "Sprint";
	}
	return "";
}

void ABaseCreature::JumpPress()
{
		BP_JumpPress();
		FString ActionString = "Jump" + GetMovementModeString("_") + GetSubMovementModeString("_");
		Jump();
		PlayMontage(ActionString);
		FallingTime = 0;
}

void ABaseCreature::Sprinting()
{
	UE_LOG(LogTemp, Warning, TEXT("ABaseCreature::Sprinting"));
	if (GetMesh()->GetAnimInstance()->IsAnyMontagePlaying()) return;
	bSprinting = true;
	if (IsGround())
	{
		SetMovement(3, 1.5);
	}
}

void ABaseCreature::StopSprinting()
{
	UE_LOG(LogTemp, Warning, TEXT("ABaseCreature::StopSprinting"));
	bSprinting = false;
	if (IsGround())
	{
		SetMovement(2, 2);
	}
}

void ABaseCreature::N1Attack()
{
	if (IsValid(Weapon))
	{
		PlayMontage("N1Attack", "", Weapon->BasePlayRate);
	}
	else
	{
		PlayMontage("N1Attack");
	}
}

void ABaseCreature::N2Attack()
{
	if (IsValid(Weapon))
	{
		PlayMontage("N2Attack", "", Weapon->BasePlayRate);
	}
	else
	{
		PlayMontage("N2Attack");
	}
}

void ABaseCreature::UseDeputy()
{
	UE_LOG(LogTemp, Warning, TEXT("ABaseCreature::ResetSave"));
	PlayMontage("UseDeputy");
}

void ABaseCreature::Lock()
{
	bLocking = !bLocking;
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
		else if (AnimInstance->ForwardSpeed < 0)
		{
			PlayMontage("Dodge", "Backward");
		}
		else if (AnimInstance->RightSpeed < -100)
		{
			PlayMontage("Dodge", "Left");
		}
		else if (AnimInstance->RightSpeed > 100)
		{
			PlayMontage("Dodge", "Right");
		}
		else
		{
			PlayMontage("Dodge", "Forward");
		}
	}
}

void ABaseCreature::FarView()
{
	if (IsValid(SpringArm))
	{
		if (SpringArm->TargetArmLength < BaseSpringArmLength * 2)
		{
			SpringArm->TargetArmLength += 0.1 * BaseSpringArmLength;
		}
	}
}

void ABaseCreature::NearView()
{
	if (IsValid(SpringArm))
	{
		if (SpringArm->TargetArmLength > 0)
		{
			SpringArm->TargetArmLength -= 0.1 * BaseSpringArmLength;
		}
	}
}

void ABaseCreature::ResetSave()
{
	UE_LOG(LogTemp, Warning, TEXT("ABaseCreature::ResetSave"));
	GetCharacterMovement()->StopActiveMovement();
	for (auto& Skill : SkillsOnUsing)
	{
		if (IsValid(Skill))
		{
			Skill->K2_DestroyActor();
		}
	}
	SkillsOnUsing.Empty();
	BP_ResetSave();
}

void ABaseCreature::Communicate()
{
	UE_LOG(LogTemp, Warning, TEXT("ABaseCreature::Communicate"));

	BP_Communicate();
}

void ABaseCreature::StopCommunicate()
{
	UE_LOG(LogTemp, Warning, TEXT("ABaseCreature::StopCommunicate"));
	BP_StopCommunicate();
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
	APlayerController* const PC =  Cast<APlayerController>(Controller);
	float Pitch = PC->GetControlRotation().Pitch;

	if (Pitch > 90)
	{
		Pitch = Pitch - 360;
		//UE_LOG(LogTemp, Warning, TEXT("ABaseCreature::LookUp Pitch %f"), Pitch);
	}

	if (Amount < 0 && Pitch < MaxLookupPitch)
	{
		AddControllerPitchInput(Amount);
	}
	if (Amount > 0 && Pitch > MinLookupPitch)
	{
		AddControllerPitchInput(Amount);
	}
}

bool ABaseCreature::CanBeIntrude(ABaseCreature* Intruder)
{
	//UE_LOG(LogTemp, Warning, TEXT("ABaseCreature::CanBeIntrude"));

	// 未被玩家控制过，且死过一次
	if (!bBeenControlled && CurHealth <= 0)
	{
		//UE_LOG(LogTemp, Warning, TEXT("ABaseCreature::CanBeIntrude bDead"));
		return true;
	}
	// 被玩家控制过，且主动脱离
	else if (bBeenControlled && CurHealth > 0)
	{
		//UE_LOG(LogTemp, Warning, TEXT("ABaseCreature::CanBeIntrude bBeenControlled"));
		return true;
	}
	// 相同阵营
	else if (IsValid(Intruder) && SameFaction(Intruder))
	{
		//UE_LOG(LogTemp, Warning, TEXT("ABaseCreature::CanBeIntrude Faction"));
		return !IsTrueDead();
	}
	// 精灵相关
	else if (CreatureID == "000") 
	{
		return true;
	}

	return false;
}


bool ABaseCreature::IsDead()
{
	return CurHealth <= 0;
}

bool ABaseCreature::IsTrueDead()
{
	return bBeenControlled && IsDead();
}

void ABaseCreature::MontageStart(UAnimMontage* Montage)
{
	UE_LOG(LogTemp, Warning, TEXT("MontageStart"));
	BP_MontageStart(Montage);
}

void ABaseCreature::MontageEnd(UAnimMontage* Montage, bool bInterrupted)
{
	UE_LOG(LogTemp, Warning, TEXT("MontageEnd"));
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

bool ABaseCreature::DelStamina(float Stamina)
{
	if (Stamina <= CurStamina)
	{
		CurStamina -= Stamina;
		return true;
	}
	return false;
}

void ABaseCreature::SetInvincible(float Time, bool bForce)
{
	UWorld* World = GetWorld();
	if (World) {
		if (bForce)
		{
			World->GetTimerManager().ClearTimer(InvincibleTimer);
			World->GetTimerManager().SetTimer(InvincibleTimer, Time, false);
		}
		else
		{
			if (!World->GetTimerManager().IsTimerActive(InvincibleTimer)) {
				World->GetTimerManager().SetTimer(InvincibleTimer, Time, false);
			}
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
	//UE_LOG(LogTemp, Warning, TEXT("ABaseCreature::SetTarget: %s"), *C->GetName());
	if (C != this)
	{
		Target = C;
	}
}
ABaseCreature* ABaseCreature::GetTarget()
{
	return Target;
}

// Creature 默认为当前的Target
void ABaseCreature::IntrudeTarget(ABaseCreature* Creature)
{
	if (IsValid(Creature))
	{
		Target = Creature;
	}
	if(!IsValid(Target)) return;

	if (Target->CanBeIntrude(this))
	{
		IntrudingTarget = Target;
		BP_IntrudeTarget();

		//if (IsValid(C_SpriteIntrudePawn))
		//{
		//	FTransform PT = GetTransform_ProjectileToTarget();
		//	PT.SetLocation(GetActorLocation());
		//	ABaseSkill* SpriteIntrudePawn = GetWorld()->SpawnActor<ABaseSkill>(C_SpriteIntrudePawn, PT);
		//	if (SpriteIntrudePawn)
		//	{
		//		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		//		PlayerController->SetViewTargetWithBlend(SpriteIntrudePawn, 1);
		//		// 到头来还是要在新建的IntrudeSpriteAIController设定OnMoveCompeleted方法。
		//		UAIBlueprintHelperLibrary::CreateMoveToProxyObject(GetWorld(), SpriteIntrudePawn, FVector(), Target);
		//	}
		//}
	}
}

bool ABaseCreature::ActionDelStamina(FString ActionName)
{
	UE_LOG(LogTemp, Warning, TEXT("ABaseCreature::ActionDelStamina ActionName: %s"), *ActionName);
	if (ActionName == "Stiff")
	{
		return true;
	}


	if (ActionName == "Dodge")
	{
		if (DelStamina(10))
		{
			SetInvincible(0.3);
			return true;
		}
	}
	else if(IsValid(Weapon))
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
			UE_LOG(LogTemp, Warning, TEXT("ABaseCreature::ActionDelStamina ActionName: 1"));
			return DelStamina(Weapon->BaseDelStamina * N1Multi);
		}
		else if (ActionName == "N2Attack")
		{
			UE_LOG(LogTemp, Warning, TEXT("ABaseCreature::ActionDelStamina ActionName: 2"));
			return DelStamina(Weapon->BaseDelStamina * N2Multi);
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("ABaseCreature::ActionDelStamina ActionName: 3"));
	return DelStamina(MaxStamina * 0.1);
}

bool ABaseCreature::PlayMontage(FString Rowname, FString SectionName,  float PlayRate)
{
	UE_LOG(LogTemp, Warning, TEXT("ABaseCreature::PlayMontage Rowname: %s"), *Rowname);
	if (Rowname == "")	return false;
	TArray<FString> MontageCanPlayDead = {"Dead", "Revive"};
	if (!IsValid(CreatureMontageDataTable)) return false;
	int _Index = Rowname.Find("_");
	FString ActionName;
	if (_Index > -1)
	{
		ActionName = Rowname.Left(_Index);
	}
	else
	{
		ActionName = Rowname;
	}


	FCreatureMontage* CreatureMontage = CreatureMontageDataTable->FindRow<FCreatureMontage>(*Rowname, TEXT("Montage"));
	if (CreatureMontage && CreatureMontage->Montage) 
	{
		if (!MontageCanPlayDead.Contains(Rowname))
		{
			if (!CanAction || IsDead()) return false;
			if (!ActionDelStamina(ActionName)) return false;
		}

		int32 Num = CreatureMontage->Montage->CompositeSections.Num();
		if (Num > 0)
		{
			int32 Index = 0;
			if (ComboIndex.Contains(ActionName))
			{
				Index = ComboIndex[ActionName]++;
				if (ComboIndex[ActionName] >= Num)
				{
					ComboIndex[ActionName] = 0;
				}
				if (Index >= Num)
				{
					Index = 0;
				}
			}

			if (SectionName == "")
			{
				SectionName = CreatureMontage->Montage->CompositeSections[Index].SectionName.ToString();
			}
			PlayAnimMontage(CreatureMontage->Montage, PlayRate, *SectionName);

			return true;
		}
	}
	return false;
}

bool ABaseCreature::NeedQuickRotate()
{
	FRotator Rotator;
	if (IsValid(Target) && !IsPlayerControlled())
	{
		FVector SelfLocation = GetActorLocation();
		FVector TargetLocation = Target->GetActorLocation();
		// 仅平面角度，因此Z都设为0
		SelfLocation.Z = 0;
		TargetLocation.Z = 0;
		Rotator = Math::FindLookAtRotation(SelfLocation, TargetLocation);
	}
	else
	{
		Rotator = GetControlRotation();
	}

	if (IsGround())
	{	// 当处于地面时，角色所看方向与操作方向的点积小于0的话，表示在后方
		return Math::Dot_VectorVector(GetActorForwardVector(), Math::GetForwardVector(Rotator)) < 0;
	}
	else
	{
		// 空战部分，暂时不写
		return false;
	}


	return false;
}

// Target: 强制指向的对象
FTransform ABaseCreature::GetTransform_ProjectileToTarget(ABaseCreature* _Target, FName StartSocketName)
{
	FVector EndPoint;
	if (IsValid(_Target))
	{
		EndPoint = _Target->GetActorLocation();
	}
	else if (!IsPlayerControlled() && IsValid(Target))
	{
		EndPoint = Target->GetActorLocation();
	}
	else
	{
		EndPoint = Camera->GetComponentLocation() + Math::GetForwardVector(Camera->GetComponentRotation()) * 10000;
	}


	FVector Location = GetMesh()->GetSocketLocation(StartSocketName);
	FRotator Rotator = Math::FindLookAtRotation(Location, EndPoint);

	return FTransform(Rotator, Location);
}
