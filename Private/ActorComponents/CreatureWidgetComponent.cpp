// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/CreatureWidgetComponent.h"
#include "UI/BaseUserWidget.h"
#include "Pawns/BaseCreature.h"

void UCreatureWidgetComponent::InitWidget()
{
	Super::InitWidget();

	UBaseUserWidget* UW = Cast<UBaseUserWidget>(GetUserWidgetObject());
	if (IsValid(UW))
	{
		UW->SetOwner(Cast<ABaseCreature>(GetOwner()));
	}
}


void UCreatureWidgetComponent::BeginPlay()
{
	Super::BeginPlay();

	Space = EWidgetSpace::Screen;

}
