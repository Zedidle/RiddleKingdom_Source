// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Equips/BaseEquip.h"
#include "Pawns/BaseCharacter.h"

// Sets default values
ABaseEquip::ABaseEquip()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABaseEquip::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseEquip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseEquip::OnEquip(ABaseCharacter* EquipUser)
{
	User = EquipUser;
	BP_OnEquip(EquipUser);
}

void ABaseEquip::OnUseStart()
{
	BP_OnUseStart();
}

void ABaseEquip::OnUseEnd()
{
	BP_OnUseEnd();
}


