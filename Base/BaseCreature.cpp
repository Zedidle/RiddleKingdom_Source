// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCreature.h"
#include "BaseAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "../ActorComponents/CreatureWidgetComponent.h"
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
	HealthWidget->SetupAttachment(GetMesh());


	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bEnableCameraLag = true;
	SpringArm->TargetArmLength = 400;
	SpringArm->CameraRotationLagSpeed = 10;
	SpringArm->CameraLagSpeed = 5;
	SpringArm->bUsePawnControlRotation = true;
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm);
	Camera->SetRelativeLocation(FVector(80, 0, 80));

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(90, 90, 120);
	CreatureTracer = CreateDefaultSubobject<UCreatureTracer>("CreatureTracer");

}


bool ABaseCreature::IsPlayerControlling()
{
	return GetController() == UGameplayStatics::GetPlayerController(GetWorld(), 0);
}

void ABaseCreature::ActionModes()
{
	if (!bAI) return;
	if (!IsValid(GetTarget())) return;  // 没有看到角色
	if (IsDead()) return;
	ActionInterval -= DeltaSeconds;
	//UE_LOG(LogTemp, Warning, TEXT("ABaseMonster::ActionModes ActionInterval: %f"), ActionInterval);

	FVector DistVector = GetTarget()->GetActorLocation() - GetActorLocation();
	float DistXY = Math::VSizeXY(DistVector); // 计算与角色的水平距离
	float DistZ = DistVector.Z; //计算与角色的垂直距离

	if (ActionInterval <= 0)
	{
		BP_ActionModes(); // 先采取蓝图的行动，有可能增加ActionInterval
	}

	if (ActionInterval < 0)  // 采取行动
	{
		UE_LOG(LogTemp, Warning, TEXT("ABaseMonster::ActionModes TakeAction"));
		ActionInterval = 1;
	}
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
	BaseSpringArmLength = SpringArm->TargetArmLength;
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
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
	bAI = !IsPlayerControlling();
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
	if (!IsValid(GetTarget()))
	{
		return EDistance::E_FLAT_SFAR;
	}


	FVector DistVector = GetTarget()->GetActorLocation() - GetActorLocation();
	float DistXY = Math::VSizeXY(DistVector); // 计算与角色的水平距离
	float DistZ = DistVector.Z; //计算与角色的垂直距离
	if (DistZ < 0)
	{
		DistZ = -DistZ;
	}
	//UE_LOG(LogTemp, Warning, TEXT("DistZ: %f"), DistZ);
	//UE_LOG(LogTemp, Warning, TEXT("DistXY: %f"), DistXY);

	if (IsGround())
	{
		if (0 <= DistXY && DistXY < 300)
		{
			return EDistance::E_FLAT_SNEAR;
		}
		else if (300 <= DistXY && DistXY < 500)
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
	SetMovement(2,2,EMovementMode::MOVE_Falling);
	PlayMontage("Dead");
	BP_Dead();
}

void ABaseCreature::Revive()
{
	UE_LOG(LogTemp, Warning, TEXT("ABaseCreature::Revive"));
	CurHealth = MaxHealth;
	if (UGameplayStatics::GetPlayerCharacter(GetWorld(), 0) == this)
	{
		bAI = false;
		EnableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));  // 暂时这样，这个0还有待商议
	}
	IsLocking = false;
	GetMesh()->GetAnimInstance()->Montage_Resume(nullptr);
	PlayMontage("Revive");
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
		CurHealth = Math::Min(CurHealth, MaxHealth);
		return 0;
	}


	if (IsInvincible() || IsDead()) return 0;
	float TrueDefense = Math::Max(Defense - Penetrate, 0);
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
	if (IsSprinting)
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
	IsSprinting = true;
	if(IsGround())
	{
		SetMovement(3, 1.5);
	}
}

void ABaseCreature::StopSprinting()
{
	UE_LOG(LogTemp, Warning, TEXT("ABaseCreature::StopSprinting"));
	IsSprinting = false;
	if (IsGround())
	{
		SetMovement(2, 2);
	}
}

void ABaseCreature::N1Attack()
{
	PlayMontage("N1Attack");
}

void ABaseCreature::N2Attack()
{
	PlayMontage("N2Attack");
}

void ABaseCreature::UseDeputy()
{
	PlayMontage("UseDeputy");
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
		else if (AnimInstance->RightSpeed < -100)
		{
			PlayMontage("Dodge", "Left");
		}
		else if (AnimInstance->RightSpeed > 100)
		{
			PlayMontage("Dodge", "Right");
		}
		else if (AnimInstance->ForwardSpeed < -100)
		{
			PlayMontage("Dodge", "Backward");
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
	AddControllerPitchInput(Amount);
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
	UE_LOG(LogTemp, Warning, TEXT("ABaseCreature::SetTarget: %s"), *C->GetName());
	Target = C;
}
ABaseCreature* ABaseCreature::GetTarget()
{
	return Target;
}



bool ABaseCreature::PlayMontage(FString Rowname, FString SectionName,  float PlayRate)
{
	UE_LOG(LogTemp, Warning, TEXT("ABaseCreature::PlayMontage Rowname: %s"), *Rowname);
	if (Rowname == "")	return false;
	//UE_LOG(LogTemp, Warning, TEXT("ABaseCreature::PlayMontage Rowname: 000"));
	TArray<FString> MontageCanPlayDead = {"Dead", "Revive"};
	if (!MontageCanPlayDead.Contains(Rowname) && !CanAction) return false;
	if (!MontageCanPlayDead.Contains(Rowname) && IsDead()) return false;
	//UE_LOG(LogTemp, Warning, TEXT("ABaseCreature::PlayMontage 111"));
	if (!IsValid(CreatureMontageDataTable)) return false;
	//UE_LOG(LogTemp, Warning, TEXT("ABaseCreature::PlayMontage 222"));
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
	UE_LOG(LogTemp, Warning, TEXT("ABaseCreature::PlayMontage ActionName: %s"), *ActionName);

	FCreatureMontage* CreatureMontage = CreatureMontageDataTable->FindRow<FCreatureMontage>(*Rowname, TEXT("Montage"));
	if (CreatureMontage && CreatureMontage->Montage) {
		int32 Num = CreatureMontage->Montage->CompositeSections.Num();
		UE_LOG(LogTemp, Warning, TEXT("ABaseCreature::PlayMontage Num: %d"), Num);

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
				UE_LOG(LogTemp, Warning, TEXT("ABaseCreature::PlayMontage 000 Index: %d"), Index);
				if (Index >= Num)
				{
					Index = 0;
				}
				UE_LOG(LogTemp, Warning, TEXT("ABaseCreature::PlayMontage 111 Index: %d"), Index);
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
	if (IsValid(Target) && bAI)
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

FTransform ABaseCreature::GetTransform_ProjectileToTarget()
{
	FVector EndPoint;
	if (bAI && IsValid(Target))
	{
		EndPoint = Target->GetActorLocation();
	}
	else
	{
		EndPoint = Camera->GetComponentLocation() + Math::GetForwardVector(Camera->GetComponentRotation()) * 10000;
	}

	FVector Location = GetMesh()->GetSocketLocation(FName("Fire_Start"));
	FRotator Rotator = Math::FindLookAtRotation(Location, EndPoint);

	return FTransform(Rotator, Location);
}
