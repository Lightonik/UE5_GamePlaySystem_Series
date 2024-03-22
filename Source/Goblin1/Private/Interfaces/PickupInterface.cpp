// Fill out your copyright notice in the Description page of Project Settings.


#include "Interfaces/PickupInterface.h"

// Add default functionality here for any IPickupInterface functions that are not pure virtual.

void IPickupInterface::SetOverlappingItem(AItem* Item)
{
}

void IPickupInterface::AddOverlappingItem(AItem* Item)
{
}

void IPickupInterface::RemoveOvelappingItem(AItem* Item)
{
}

void IPickupInterface::PickupItem(AItem* Item)
{
}

void IPickupInterface::PickUpOverlappingItem(AItem* Item)
{
}

bool IPickupInterface::IsPickedup(AItem* Item)
{
	return false;
}

void IPickupInterface::MoveToTarget(AActor* Tatget)
{
}
