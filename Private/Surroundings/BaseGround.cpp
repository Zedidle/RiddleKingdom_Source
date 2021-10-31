// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

#include "Surroundings/BaseGround.h"

// Sets default values
ABaseGround::ABaseGround()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	Box = CreateDefaultSubobject<UBoxComponent>("Box");
	Box->SetupAttachment(StaticMesh);
	Box->SetRelativeScale3D(FVector(1, 1, 0.001));
}

// Called when the game starts or when spawned
void ABaseGround::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseGround::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FString ABaseGround::GroundTypeToTableRowname()
{
	TMap<EGroundType, FString> GroundTypeToTableRowname;
	GroundTypeToTableRowname.Add(EGroundType::E_NONE, "");
	GroundTypeToTableRowname.Add(EGroundType::E_GRASS, "Grass");
	GroundTypeToTableRowname.Add(EGroundType::E_ROCK, "Rock");
	GroundTypeToTableRowname.Add(EGroundType::E_SAND, "Sand");
	GroundTypeToTableRowname.Add(EGroundType::E_SNOW, "Snow");
	return GroundTypeToTableRowname[GroundType];
}

