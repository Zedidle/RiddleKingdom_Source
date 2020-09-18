// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseNpc.h"
#include "BaseCharacter.h"

// Sets default values
ABaseNpc::ABaseNpc()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABaseNpc::BeginPlay()
{
	Super::BeginPlay();
	
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


void ABaseNpc::Welcome(ABaseCharacter* Character)
{
	CurCharacter = Character;
}

void ABaseNpc::Communicate()
{
	BP_Communicate();
}

