// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseMonster.h"
#include "Perception/PawnSensingComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Delegates/DelegateInstancesImpl.h"





// Sets default values
ABaseMonster::ABaseMonster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//Sensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Sensing")); 
}

// Called when the game starts or when spawned
void ABaseMonster::BeginPlay()
{
	Super::BeginPlay();
	
}



// Called every frame
void ABaseMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(IsDead()) return;
	ActionModes(DeltaTime);
	CalNearTime(DeltaTime);
}

// Called to bind functionality to input
void ABaseMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

bool ABaseMonster::IsDead()
{
	return CurHealth <= 0;
}

void ABaseMonster::Dead()
{
	BP_Dead();
}

void ABaseMonster::AcceptDamage(float Damage, float Penetrate)
{
	if (IsInvincible()) return;
	if (IsDead()) return;
	UE_LOG(LogTemp, Warning, TEXT("ABaseMonster::AcceptDamage Damage:%f"), Damage);
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
		AngerCount += TrueDamage;
	}
	BP_AcceptDamage();
}

void ABaseMonster::Welcome(ABaseCharacter* Character)
{
	CurCharacter = Character;
	Character->SetCurBoss(this);
	UE_LOG(LogTemp, Warning, TEXT("ABaseMonster::Welcome"));
}

void ABaseMonster::ActionModes(float DeltaTime)
{
	if(!IsValid(CurCharacter)) return;  // 没有看到角色
	ActionInterval -= DeltaTime;

	//UE_LOG(LogTemp, Warning, TEXT("ActionInterval: %f"), ActionInterval);

	FVector DistVector = CurCharacter->GetActorLocation() - GetActorLocation();
	float DistXY = UKismetMathLibrary::VSizeXY(DistVector); // 计算与角色的水平距离
	float DistZ = DistVector.Z; //计算与角色的垂直距离

	if (ActionInterval < 0)
	{
		BP_ActionModes(); // 先采取蓝图的行动，有可能增加ActionInterval
	}

	if (ActionInterval < 0)  // 采取行动
	{
		
		UE_LOG(LogTemp, Warning, TEXT("TakeAction"));
		ActionInterval = 4;


		// 根据这两个距离行动
		//UE_LOG(LogTemp, Warning, TEXT("ABaseMonster::ActionModes %f, %f"), (DistXY, DistZ));
	}
}

EDistance ABaseMonster::GetDistanceTypeToCharacter()
{
	if (!IsValid(CurCharacter))
	{
		return EDistance::E_FLAT_SFAR;
	}
	

	FVector DistVector = CurCharacter->GetActorLocation() - GetActorLocation();
	float DistXY = UKismetMathLibrary::VSize(DistVector); // 计算与角色的水平距离
	float DistZ = DistVector.Z; //计算与角色的垂直距离

	if (200 <= DistZ && DistZ < 500)
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
	else if (0 <= DistXY && DistXY < 700)
	{
		return EDistance::E_FLAT_NEAR;
	}
	else if (700 <= DistXY && DistXY < 1500)
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
	return EDistance::E_FLAT_NEAR;
}

void ABaseMonster::CalNearTime(float DeltaTime)
{
	if(!IsValid(CurCharacter)) return;

	if (GetDistanceTypeToCharacter() == EDistance::E_FLAT_NEAR)
	{
		NearTime += DeltaTime;
	}
	else
	{
		NearTime = 0;
	}
}

FRotator ABaseMonster::GetRotationToCharacter()
{
	if (!IsValid(CurCharacter))
	{
		return FRotator();
	}
	else
	{
		FVector V = CurCharacter->GetActorLocation() - GetActorLocation();
		V.Z = 0;
		return UKismetMathLibrary::MakeRotationFromAxes(V, FVector(0), FVector(0));
	}
}

bool ABaseMonster::RotateToCharacter()
{
	if (IsValid(CurCharacter))
	{
		FRotator Rotation = GetRotationToCharacter();
		
		AddMovementInput(UKismetMathLibrary::GetForwardVector(Rotation), 0.1);
		return true;
	}
	return false;
}

void ABaseMonster::SetMaxWalkSpeed(float Speed)
{
	GetCharacterMovement()->MaxWalkSpeed = Speed;
}

//void ABaseMonster::AddMonsterAction(EDistance Distance, float ActionRate, float ActionTime, FMethodPtr Func)
//void ABaseMonster::AddMonsterAction(EDistance Distance, float ActionRate, float ActionTime, Funcptr Func)
//{
//	FMonsterAction Action;
//	Action.ActionDistance = Distance;
//	Action.ActionRate = ActionRate;
//	Action.ActionTime = ActionTime;
//	Action.Func = Func;
//	MonsterActionSet.Add(Action);
//}

void ABaseMonster::GetMonsterActionsByDistance(EDistance Distance)
{
	EDistance DistanceType = GetDistanceTypeToCharacter();
	for (auto& Action : MonsterActionSet)
	{

		FPlatformMisc::LocalPrint(
			*FString::Printf(
				TEXT(" \"11%s\"\n"),
				Action.ActionTime
			)
		);
		if (Distance == Action.ActionDistance)
		{
			FPlatformMisc::LocalPrint(
				*FString::Printf(
					TEXT(" \"22%s\"\n"),
					Action.ActionRate
				)
			);
		}
	}

}

void ABaseMonster::SetInvincible(float Time)
{
	UWorld* World = GetWorld();
	if (World) {
		if (!World->GetTimerManager().IsTimerActive(InvincibleTimer)) {
			World->GetTimerManager().SetTimer(InvincibleTimer, Time, false);
		}
	}
}

bool ABaseMonster::IsInvincible()
{
	UWorld* World = GetWorld();
	return World && World->GetTimerManager().IsTimerActive(InvincibleTimer);
}

float ABaseMonster::GetHealthPercent()
{
	return CurHealth / MaxHealth;
}

bool ABaseMonster::IsAnger()
{
	return (AngerCount / MaxHealth) > AngerSpawn;
}

void ABaseMonster::SetAngerCount(float Count)
{
	AngerCount = Count;
}

