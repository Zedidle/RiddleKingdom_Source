// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseMonster.h"
#include "BaseGameInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Delegates/DelegateInstancesImpl.h"
#include "Components/InputComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "Kismet/KismetMathLibrary.h"
using Math = UKismetMathLibrary;

// Sets default values
ABaseMonster::ABaseMonster()
{
	PrimaryActorTick.bCanEverTick = true;
	Faction = EFaction::E_Monster; // 设置阵营
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
	PawnSensingComp->SetPeripheralVisionAngle(80);
}

// Called when the game starts or when spawned
void ABaseMonster::BeginPlay()
{
	Super::BeginPlay();
	SetMovement(2,2);
	PawnSensingComp->OnSeePawn.AddDynamic(this, &ABaseMonster::OnSeePawn);
}



// Called every frame
void ABaseMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(IsDead()) return;
	ActionModes();
	CalNearTime();
}



void ABaseMonster::Dead()
{
	Super::Dead();
	UE_LOG(LogTemp, Warning, TEXT("ABaseMonster::Dead"));
	Target = nullptr;
	UBaseGameInstance* GI = Cast<UBaseGameInstance>(GetGameInstance());
	if (IsValid(GI) && MonsterLevel == EMonsterLevel::E_Boss)
	{
		UE_LOG(LogTemp, Warning, TEXT("ABaseMonster::Dead  111"));
		GI->SetCurBoss(nullptr);
	}
}

void ABaseMonster::OnSeePawn(APawn* Pawn)
{
	UE_LOG(LogTemp, Warning, TEXT("ABaseMonster::OnSeePawn"));
	if (IsDead()) return;
	ABaseCreature* C = Cast<ABaseCreature>(Pawn);
	if (IsValid(C))
	{
		SetTarget(C);
	}
}

void ABaseMonster::OnAttackChanged(bool Enable)
{
}

void ABaseMonster::OnAttackDamageEnableChanged(bool Enable)
{
	//BP_OnAttackDamageEnableChanged(Enable);
}

void ABaseMonster::OnAttackComboEnableChanged(bool Enable)
{
}


float ABaseMonster::AcceptDamage(float Damage, float Penetrate)
{
	UE_LOG(LogTemp, Warning, TEXT("ABaseMonster::AcceptDamage"));
	float TrueDamage = Super::AcceptDamage(Damage, Penetrate);
	AngerCount += TrueDamage;
	StiffCount += TrueDamage;
	Stiff();
	return TrueDamage;
}

void ABaseMonster::ActionModes()
{
	if(!IsValid(GetTarget())) return;  // 没有看到角色
	if(IsDead()) return;
	ActionInterval -= DeltaSeconds;

	UE_LOG(LogTemp, Warning, TEXT("ABaseMonster::ActionModes ActionInterval: %f"), ActionInterval);

	FVector DistVector = GetTarget()->GetActorLocation() - GetActorLocation();
	float DistXY = UKismetMathLibrary::VSizeXY(DistVector); // 计算与角色的水平距离
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


void ABaseMonster::CalNearTime()
{
	if(!IsValid(GetTarget())) return;

	if (GetDistanceTypeToTarget() == EDistance::E_FLAT_NEAR)
	{
		NearTime += DeltaSeconds;
	}
	else
	{
		NearTime = 0;
	}
}

FRotator ABaseMonster::GetRotationToCharacter()
{
	if (!IsValid(GetTarget()))
	{
		return FRotator();
	}
	else
	{
		FVector V = GetTarget()->GetActorLocation() - GetActorLocation();
		V.Z = 0;
		return UKismetMathLibrary::MakeRotationFromAxes(V, FVector(0), FVector(0));
	}
}

bool ABaseMonster::RotateToCharacter()
{
	if (IsValid(GetTarget()))
	{
		FRotator Rotation = GetRotationToCharacter();
		
		AddMovementInput(UKismetMathLibrary::GetForwardVector(Rotation), 0.1);
		return true;
	}
	return false;
}

void ABaseMonster::Stiff()
{
	if(IsDead()) return;

	if ((StiffCount / MaxHealth) > StiffSpawn)
	{
		PlayMontage("Stiff");
		StiffCount = 0;
	}
}


bool ABaseMonster::IsAnger()
{
	return (AngerCount / MaxHealth) > AngerSpawn;
}

void ABaseMonster::SetAngerCount(float Count)
{
	AngerCount = Count;
}
void ABaseMonster::SetStiffCount(float Count)
{
	StiffCount = Count;
}


void ABaseMonster::SetTarget(ABaseCreature* C)
{
	if(IsValid(Target)) return;
	Super::SetTarget(C);
	//UE_LOG(LogTemp, Warning, TEXT("ABaseMonster::SetTarget 111"));
	UBaseGameInstance* GI = Cast<UBaseGameInstance>(GetGameInstance());
	if (IsValid(GI) && EMonsterLevel::E_Boss == MonsterLevel)
	{
		//UE_LOG(LogTemp, Warning, TEXT("ABaseMonster::SetTarget 222"));
		GI->SetCurBoss(this);
	}
}