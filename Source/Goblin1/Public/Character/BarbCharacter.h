// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterTypes.h"
#include "BaseCharacter.h"
#include "Interfaces/PickupInterface.h"

#include "BarbCharacter.generated.h"



class USpringArmComponent;
class UCameraComponent;
class AItem;
class UAnimMontage;
class AWeapon;
class UBarbOverlay;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;




UCLASS()
class GOBLIN1_API ABarbCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	//	<AActor>
	ABarbCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	//	</AActor>

	// <UInputComponent>
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// </UInputComponent>

	// <IHitInterface>
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	// </IHitInterface>

	// <IPickupInterface>
	virtual void PickUpOverlappingItem(AItem* Item) override;
	// </IPickupInterface>

protected:
	//	<AActor>
	virtual void BeginPlay() override;
	//	</AActor>

	// Callbacks for input
	void SetInputSystem();

	void EquipKeyPressed();
	void ActionKeyPressed();

	void DropItem(AItem* Item);

	// Combat
	virtual void Attack() override;
	virtual bool CanAttack() override;
	virtual void AttackEnd() override;
	void EquipWeapon(AWeapon* Weapon);
	bool CanDisarm();
	bool CanArm();
	bool IsUnoccupied();
	void Arm();
	void Disarm();
	virtual void Die() override;

	UFUNCTION(BlueprintCallable)
	void AttachWeaponToBack();

	UFUNCTION(BlueprintCallable)
	void AttachWeaponToHand();

	UFUNCTION(BlueprintCallable)
	void FinishEquipping();

	UFUNCTION(BlueprintCallable)
	void HitReactEnd();

	// Play montage functions
	void PlayEquipMontage(const FName& SectionName);

	//Enhanced Input
	void EnhancedInputMove(const FInputActionValue& Value);
	void EnhancedInputLook(const FInputActionValue& Value);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Input")
	TSoftObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Input")
	UInputAction* InputMove;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Input")
	UInputAction* InputLook;

	int32 Priority = 1;



private:

	void InitOverlay();
	void SetHUDHealth();

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ViewCamera;

	//Enums & States
	
	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;
	
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EActionsState ActionsState = EActionsState::ECS_Unoccupied;	

	// Animation montages

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* EquipMontage;

	UPROPERTY()
	UBarbOverlay* Overlay;

public:
//	FORCEINLINE void SetOvelappingItem(AItem* Item) { OverlappingItem = Item; }
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }
	FORCEINLINE EActionsState GetActionState() const { return ActionsState; }
};
