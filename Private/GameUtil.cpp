// Fill out your copyright notice in the Description page of Project Settings.


#include "GameUtil.h"


UGameUtil::UGameUtil(){}

void UGameUtil::SetDilation(const UWorld* World, float Scale, float Time)
{
	W = World;
	SetGlobalTimeDilation(W, Scale);
	W->GetTimerManager().ClearTimer(DilationTimer);
	W->GetTimerManager().SetTimer(DilationTimer, this, &UGameUtil::DilationTimeout, Time);
}

void UGameUtil::DilationTimeout()
{
	if (W)
	{
		SetGlobalTimeDilation(W, 1);
	}
}
