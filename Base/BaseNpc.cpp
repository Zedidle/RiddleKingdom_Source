// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseNpc.h"
#include "BaseCharacter.h"


// Sets default values
ABaseNpc::ABaseNpc()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
	PawnSensingComp->SetPeripheralVisionAngle(80);

}

// Called when the game starts or when spawned
void ABaseNpc::BeginPlay()
{
	Super::BeginPlay();
	PawnSensingComp->OnSeePawn.AddDynamic(this, &ABaseNpc::OnSeePawn);
	
}

// Called every frame
void ABaseNpc::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseNpc::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABaseNpc::OnSeePawn(APawn* Pawn)
{
	UE_LOG(LogTemp, Warning, TEXT("ABaseMonster::OnSeePawn"));
	if (IsDead()) return;
	ABaseCreature* C = Cast<ABaseCreature>(Pawn);
	if (IsValid(C))
	{
		SetTarget(C);
	}
}
