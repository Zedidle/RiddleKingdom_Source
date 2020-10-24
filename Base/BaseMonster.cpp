// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseMonster.h"
#include "BaseGameInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Delegates/DelegateInstancesImpl.h"
#include "Components/InputComponent.h"
#include "CreatureAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
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
	Tick_CalNearTime();
}

void ABaseMonster::Tick_CalNearTime()
{
	if (!IsValid(GetTarget())) return;

	switch (GetDistanceTypeToTarget())
	{
	case EDistance::E_FLAT_SNEAR:
		NearTime += DeltaSeconds;
		break;

	default:
		NearTime = 0;
		break;
	}
}



void ABaseMonster::Dead(bool bClearHealth)
{
	Super::Dead(bClearHealth);
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
	if (IsDead() || !bAI) return;
	UE_LOG(LogTemp, Warning, TEXT("ABaseMonster::OnSeePawn 111"));
	ABaseCreature* C = Cast<ABaseCreature>(Pawn);
	if (!IsValid(C) || C->IsDead()) return;
	UE_LOG(LogTemp, Warning, TEXT("ABaseMonster::OnSeePawn Name: %s"), *C->GetName());

	SetTarget(C);

	ACreatureAIController* AIController = Cast<ACreatureAIController>(GetController());
	if (IsValid(AIController))
	{
		UE_LOG(LogTemp, Warning, TEXT("ABaseMonster::OnSeePawn 333"));
		UBlackboardComponent* BB = AIController->GetBlackboardComponent();
		BB->SetValueAsObject("TargetActor", C);
		UE_LOG(LogTemp, Warning, TEXT("ABaseMonster::OnSeePawn BB->SetValueAsObject TargetActor"));
		if (Faction != C->Faction)
		{
			bCombating = true;
		}
	}
}

void ABaseMonster::OnAttackChanged(bool Enable)
{
	if (Enable)
	{
		CanAction = false;
	}
	else
	{
		CanAction = true;
	}
}

void ABaseMonster::OnAttackDamageEnableChanged(bool Enable)
{
	BP_OnAttackDamageEnableChanged(Enable);
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
	if(IsDead() || IsStoic()) return;

	if ((StiffCount / MaxHealth) > StiffSpawn)
	{
		PlayMontage("Stiff");
		StiffCount = 0;
		// 获得霸体时间
		SetStoic(5);
	}
}

void ABaseMonster::SetStoic(float Time)
{
	UWorld* World = GetWorld();
	if (World) {
		if (!World->GetTimerManager().IsTimerActive(StoicTimer)) {
			World->GetTimerManager().SetTimer(StoicTimer, Time, false);
		}
	}
}
bool ABaseMonster::IsStoic()
{
	UWorld* World = GetWorld();
	return World && World->GetTimerManager().IsTimerActive(StoicTimer);
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
	UE_LOG(LogTemp, Warning, TEXT("ABaseMonster::SetTarget"));
	// 后面还要考虑AI状态下，Target死亡后如何转移目标（仇恨值、最低生命值）
	// “当前目标死亡后才能够更换新目标”
	if (bAI && IsValid(Target) && !Target->IsDead())
	{
		return;
	}
	
	Super::SetTarget(C);
	UE_LOG(LogTemp, Warning, TEXT("ABaseMonster::SetTarget 111"));
	UBaseGameInstance* GI = Cast<UBaseGameInstance>(GetGameInstance());
	if (IsValid(GI) && EMonsterLevel::E_Boss == MonsterLevel)
	{
		//UE_LOG(LogTemp, Warning, TEXT("ABaseMonster::SetTarget Boss"));
		GI->SetCurBoss(this);
	}
}