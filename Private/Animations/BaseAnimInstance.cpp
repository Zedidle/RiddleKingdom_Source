// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/BaseAnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Pawns/BaseCreature.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"



void UBaseAnimInstance::UpdateAnimaton() 
{
	BP_UpdateAnimaton();
	ABaseCreature* Owner = Cast<ABaseCreature>(TryGetPawnOwner());
	
	if (IsValid(Owner))
	{
		// 同步角色的速度和方向、是否在空中
		FVector Velocity = Owner->GetVelocity();
		Speed = UKismetMathLibrary::VSize(Velocity);

		FRotator Rotator = Owner->GetActorRotation();
		Direction = CalculateDirection(Velocity, Rotator);

		ForwardSpeed = UKismetMathLibrary::Dot_VectorVector(Velocity, Owner->GetActorForwardVector());
		RightSpeed = UKismetMathLibrary::Dot_VectorVector(Velocity, Owner->GetActorRightVector());
		UpSpeed = UKismetMathLibrary::Dot_VectorVector(Velocity, FVector(0,0,1.f));

		IsFalling = Owner->GetCharacterMovement()->IsFalling();
		IsFlying = Owner->GetCharacterMovement()->IsFlying();
		IsLocking = Owner->bLocking;
	}
}

void UBaseAnimInstance::NativeBeginPlay()
{
	ABaseCreature* Owner = Cast<ABaseCreature>(TryGetPawnOwner());
	if (IsValid(Owner))
	{
		OnMontageStarted.AddDynamic(Owner, &ABaseCreature::MontageStart);
		OnMontageEnded.AddDynamic(Owner, &ABaseCreature::MontageEnd);
	}
}
