// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseDeputy.h"
#include "BaseCreature.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"

// Sets default values
ABaseDeputy::ABaseDeputy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMesh");
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ArrowComponent = CreateEditorOnlyDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	if (ArrowComponent)
	{
		ArrowComponent->ArrowColor = FColor(150, 200, 255);
		ArrowComponent->bTreatAsASprite = true;
		ArrowComponent->bIsScreenSizeScaled = true;
	}
}

// Called when the game starts or when spawned
void ABaseDeputy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseDeputy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

EDeputyType ABaseDeputy::GetDeputyType()
{
	return DeputyType;
}

void ABaseDeputy::SetUser(ABaseCreature* U)
{
	User = U;
}

ABaseCreature* ABaseDeputy::GetUser()
{
	return User;
}

// 需要通过使用者不同动作阶段触发
void ABaseDeputy::StartUse()
{
	BP_StartUse();
}

// 需要通过使用者不同动作阶段触发
void ABaseDeputy::EndUse()
{
	BP_EndUse();
}
