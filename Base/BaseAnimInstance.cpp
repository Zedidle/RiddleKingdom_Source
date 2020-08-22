// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "BaseCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"



void UBaseAnimInstance::UpdateAnimaton() 
{
	ABaseCharacter* Character = Cast<ABaseCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	
	if (Character)
	{
		// 同步角色的速度和方向、是否在空中
		FVector CharacterVector = Character->GetVelocity();
		FRotator CharacterRotator = Character->GetActorRotation();
		Direction = this->CalculateDirection(CharacterVector, CharacterRotator);
		Speed = UKismetMathLibrary::VSize(CharacterVector);
		IsFalling = Character->GetCharacterMovement()->IsFalling();
		CharacterActionActiveIndex = Character->ActionActiveIndex;
	}
}