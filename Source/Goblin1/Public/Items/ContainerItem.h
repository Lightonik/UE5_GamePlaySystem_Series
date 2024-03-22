// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "ContainerItem.generated.h"

class AItem;

/**
 * 
 */
UCLASS()
class GOBLIN1_API AContainerItem : public AItem
{
	GENERATED_BODY()
	
public:
	AContainerItem();

	UPROPERTY(EditInstanceOnly, Category = "Content")
	TArray<AItem*> Content;

	void PutItemToContainer(AItem* Item);

protected:


};
