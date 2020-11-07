// Fill out your copyright notice in the Description page of Project Settings.


#include "CreatureTracer.h"
#include "../Base/BaseCreature.h"
#include "../Base/BaseEquip.h"
#include "../Base/BaseGameInstance.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"


// Sets default values for this component's properties
UCreatureTracer::UCreatureTracer()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	Owner = Cast<ABaseCreature>(GetOwner());

	// ...
}


// Called when the game starts
void UCreatureTracer::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


void UCreatureTracer::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// 后面看要不要只在角色被玩家控制时才检测; IsAI的判断后面也要斟酌修改
	if (IsValid(Owner) && IsValid(Owner->Camera) && !Owner->bAI)
	{
		if(Owner->IsDead()) return;
		if(Owner->GetController() != UGameplayStatics::GetPlayerController(GetWorld(), 0)) return;

		UCameraComponent* C = Owner->Camera;
		FVector StartPoint = C->GetComponentLocation();
		FVector EndPoint = UKismetMathLibrary::GetForwardVector(C->GetComponentRotation()) * TraceLength + StartPoint;
		bool IsHit = UKismetSystemLibrary::LineTraceSingle(GetWorld(),StartPoint, EndPoint, TraceChannel, false, ActorsToIgnore, DrawDebugType, HitResult, true);
		OnHit(IsHit);
	}
}

void UCreatureTracer::OnHit(bool bHit)
{
	if (!bHit)
	{
		Creature = nullptr;
		Equip = nullptr;
	}
	else
	{
		Creature = Cast<ABaseCreature>(HitResult.Actor);
		if (IsValid(Creature))
		{
			if (!Owner->bLocking)
			{
				Owner->SetTarget(Creature);
			}
		}
		Equip = Cast<ABaseEquip>(HitResult.Actor);
	}

	BP_OnHit(bHit);
}

