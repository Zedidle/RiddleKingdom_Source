// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGameManager.h"

// Sets default values
ABaseGameManager::ABaseGameManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABaseGameManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

