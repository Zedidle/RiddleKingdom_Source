// Fill out your copyright notice in the Description page of Project Settings.


#include "CreatureTracer.h"
#include "../Base/BaseCreature.h"
#include "Camera/CameraComponent.h"
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


// Called every frame
void UCreatureTracer::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (IsValid(Owner) && IsValid(Owner->Camera) && !Owner->IsAI)
	{
		UCameraComponent* C = Owner->Camera;
		FVector StartPoint = C->GetComponentLocation();
		FVector EndPoint = UKismetMathLibrary::GetForwardVector(C->GetComponentRotation()) * TraceLength + StartPoint;
		bool IsHit = UKismetSystemLibrary::LineTraceSingle(GetWorld(),StartPoint, EndPoint, TraceChannel, false, ActorsToIgnore, DrawDebugType, HitResult, true);
		OnHit(IsHit);
	}
}

void UCreatureTracer::OnHit(bool IsHit)
{
	if(!IsHit) return;
	//UE_LOG(LogTemp, Warning, TEXT("UCreatureTracer::OnHit"));
	class ABaseCreature* C = Cast<ABaseCreature>(HitResult.Actor);
	if(IsValid(C))
	{
		Owner->SetTarget(C);
	}


}

