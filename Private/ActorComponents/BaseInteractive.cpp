// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/BaseInteractive.h"

// Sets default values
ABaseInteractive::ABaseInteractive()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABaseInteractive::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseInteractive::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseInteractive::Communicate()
{
	BP_Communicate();
}

