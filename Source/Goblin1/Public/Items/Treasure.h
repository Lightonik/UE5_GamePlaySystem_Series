// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Treasure.generated.h"

/**
 * 
 */
UCLASS()
class GOBLIN1_API ATreasure : public AItem
{
	GENERATED_BODY()
	
protected:
	//	<AItem>
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, 
		AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
		bool bFromSweep, const FHitResult& SweepResult) override;
	//	</AItem>

private:

	UPROPERTY(EditAnywhere, Category = Sounds)
	USoundBase* PickupSound;

	UPROPERTY(EditAnywhere, Category = "Treasure properties")
	int32 Gold;
};
