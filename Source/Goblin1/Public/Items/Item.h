// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"


//UENUM(BlueprintType)
enum class EItemState : uint8
{
	EIS_OnGround,
	EIS_Equipped

};

enum class EItemActions : uint8
{
	EIA_Examine,
	EIA_Pickup,
	EIA_Equip,
	EIA_Store
};



class USphereComponent;
class UNiagaraComponent;


UCLASS()
class GOBLIN1_API AItem : public AActor
{
	GENERATED_BODY()

public:

	//	<AActor>
	virtual void Tick(float DeltaTime) override;
	//	</AActor>

	AItem();

	void HideItemMesh();
	void DisableCollision();


protected:

	//	<AActor>
	virtual void BeginPlay() override;
	//	</AActor>

	void DisablePhysics();

	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* ItemMesh;

	EItemState ItemState = EItemState::EIS_OnGround;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* Sphere;

	UPROPERTY(EditAnywhere)
	UNiagaraComponent* EmbersEffect;  

	// Interactions
	UPROPERTY(VisibleAnywhere)
	TArray<ACharacter*> OverlappedByCharacter;

	UPROPERTY(VisibleAnywhere)
	TArray<ACharacter*> UsedByCharacter;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float RuningTime = 0.f;
	float SphereRadius = 64.f;
	bool bHaveCollisions = true;
	int32 Priority = 0;

public:

	FORCEINLINE void AddNewToUsedBy(ACharacter* Character) { UsedByCharacter.AddUnique(Character); }
	FORCEINLINE int32 GetPriority() { return Priority; }

};
