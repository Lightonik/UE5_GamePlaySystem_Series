// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "Interfaces/PickupInterface.h"
#include "Character/CharacterTypes.h"

//#include "Character/Interactions/AAction.h"


#include "BaseCharacter.generated.h"

class UAnimMontage;
class UAttributeComponent;
class UActionComponent;
class AItem;
class AWeapon;

UCLASS()
class GOBLIN1_API ABaseCharacter : public ACharacter, public IHitInterface, public IPickupInterface
{
	GENERATED_BODY()

public:
	//	<AActor>
	ABaseCharacter();
	virtual void Tick(float DeltaTime) override;
	//	</AActor>

	UPROPERTY(EditAnywhere, Category = Character)
	FName Name;

	UPROPERTY(EditAnywhere, Category = Character)
	int32 ID;

	// <IPickupInterface>
	virtual void SetOverlappingItem(AItem* Item) override;
	virtual void AddOverlappingItem(AItem* Item) override;
	virtual void RemoveOvelappingItem(AItem* Item) override;
	virtual void PickupItem(AItem* Item) override;
	virtual void PickUpOverlappingItem(AItem* Item) override;
	virtual bool IsPickedup(AItem* Item) override;

	UFUNCTION(BlueprintCallable)
	virtual void MoveToTarget(AActor* Target) override;

	// </IPickupInterface>
protected:
	//	<AActor>
	virtual void BeginPlay() override;
	//	</AActor>

	// <IHitInterface>
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	// </IHitInterface>

	// AI Behavior
	void EquipWeapon(AWeapon* Weapon);



	UPROPERTY()
	class AAIController* CharacterController;



	// Combat 
	virtual void Attack();
	virtual bool CanAttack();
	virtual void HandleDamage(float DamageAmount);
	virtual void Die();
	bool IsAlive();
	void DisableCapsule();
	void InitCapsule();
	void PlayHitSound(const FVector& ImpactPoint);
	void SpawnHitParticles(const FVector& ImpactPoint);

	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd();
	UFUNCTION(BlueprintCallable)
	void SetWeaponCollision(ECollisionEnabled::Type CollisionEnabled);

	UPROPERTY(BlueprintReadOnly, Category = Combat)
	AActor* CombatTarget;
	
	UPROPERTY(EditAnywhere, Category = Combat)
	double WarpTargetDistance = 75.f;

	// Play montage functions
	virtual int32 PlayAttackMontage();
	virtual int32 PlayDeathMontage();
	void PlayHitReactMontage(const FName& SectionName);
	void DirectionalHitReact(const FVector& ImpactPoint);
	void StopAttackMontage();

	// Warp animation
	UFUNCTION(BlueprintCallable)
	FVector GetTranslationWarpTarget();
	UFUNCTION(BlueprintCallable)
	FVector GetRotationWarpTarget();

	// Components
	UPROPERTY(VisibleAnywhere)
	UAttributeComponent* Attributes;
	
	UPROPERTY(VisibleAnywhere)
	UActionComponent* ActionList;


	UPROPERTY(VisibleAnywhere, Category = Weapon)
	AWeapon* EquippedWeapon;
	
	UPROPERTY(VisibleInstanceOnly, Category = Items)
	TArray<AItem*> WearablesItems;

	UPROPERTY(VisibleInstanceOnly, Category = Items)
	AItem* OverlappingItem;

	UPROPERTY(VisibleInstanceOnly, Category = Items)
	TArray<AItem*> OverlappingItems;

	// Enums
	UPROPERTY(BlueprintReadOnly)
	TEnumAsByte<EDeathPose> DeathPose;


private:	



	// Play montage functions
	void PlayMontageSection(UAnimMontage* Montage, const FName& SectionName);
	int32 PlayRandomMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionNames);

	UPROPERTY(EditAnywhere, Category = "Sounds and Visual Effects")
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = "Sounds and Visual Effects")
	UParticleSystem* HitParticles;

	// Animation montages

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* DeathMontage;

	UPROPERTY(EditAnywhere, Category = Montages)
	TArray<FName> AttackMontageSections;

	UPROPERTY(EditAnywhere, Category = Montages)
	TArray<FName> DeathMontageSections;

public:

	//FORCEINLINE void SetOvelappingItem(AItem* Item) { OverlappingItem = Item; }	
	//FORCEINLINE void AddOverlappingItem(AItem* Item) { OverlappingItems.AddUnique(Item); }
	FORCEINLINE TEnumAsByte<EDeathPose> GetDeathPose() const { return DeathPose; }



};
