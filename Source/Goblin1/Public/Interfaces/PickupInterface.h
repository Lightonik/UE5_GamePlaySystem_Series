// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PickupInterface.generated.h"

class AItem;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPickupInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GOBLIN1_API IPickupInterface
{
	GENERATED_BODY()

public:
	virtual void SetOverlappingItem(AItem* Item);
	virtual void AddOverlappingItem(AItem* Item);
	virtual void RemoveOvelappingItem(AItem* Item);
	virtual void PickupItem(AItem* Item);
	virtual void PickUpOverlappingItem(AItem* Item);

	virtual bool IsPickedup(AItem* Item);

	virtual void MoveToTarget(AActor* Tatget);



};
