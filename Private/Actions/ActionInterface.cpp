// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/ActionInterface.h"
#include "Utils/BaseDefinedStructs.h"


// 暂时不知道如何用这种代理方法，索性BBTs堆上去。
void IActionInterface::ActionDelegate(EDistance D)  
{
	UE_LOG(LogTemp, Warning, TEXT("IActionInterface::ActionDelegate"));
	switch (D)
	{
	case EDistance::E_PLUMB_NEAR:
		ActionPlumbNear();
		break;
	case EDistance::E_PLUMB_MID:
		ActionPlumbMid();
		break;
	case EDistance::E_PLUMB_FAR:
		ActionPlumbFar();
		break;
	case EDistance::E_PLUMB_SFAR:
		ActionPlumbSFar();
		break;
	case EDistance::E_FLAT_NEAR:
		ActionFlatNear();
		break;
	case EDistance::E_FLAT_MID:
		ActionFlatMid();
		break;
	case EDistance::E_FLAT_FAR:
		ActionFlatFar();
		break;
	case EDistance::E_FLAT_SFAR:
		ActionFlatSFar();
		break;
	default:
		break;
	}
}
