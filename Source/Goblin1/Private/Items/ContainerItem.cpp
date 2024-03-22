// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ContainerItem.h"
#include "Items/Item.h"
#include "Components/SceneComponent.h"

AContainerItem::AContainerItem()
{
	ItemMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);

}

void AContainerItem::PutItemToContainer(AItem* Item)
{
	Item->HideItemMesh();
	Content.Add(Item);
}
