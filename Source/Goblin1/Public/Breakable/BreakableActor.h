// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Hitinterface.h"


#include "BreakableActor.generated.h"


class UGeometryCollectionComponent;

UCLASS()
class GOBLIN1_API ABreakableActor : public AActor, public IHitInterface
{
	GENERATED_BODY()
	
public:	
	ABreakableActor();
	virtual void Tick(float DeltaTime) override;
	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;


protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void BreakEvent(const FChaosBreakEvent& BreakEvent);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UGeometryCollectionComponent* GeometryCollection;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UCapsuleComponent* Capsule;

private:

	UPROPERTY(EditAnywhere, Category = "Breakable Properties")
	TArray<TSubclassOf<class ATreasure>>TreasureClasses;

	bool bBroken = false;

public:	

};
