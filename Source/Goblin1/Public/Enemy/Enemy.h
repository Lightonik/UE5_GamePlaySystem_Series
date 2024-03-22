// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter.h"
#include "Interfaces/Hitinterface.h"
#include "Character/CharacterTypes.h"
#include "Enemy.generated.h"




UCLASS()
class GOBLIN1_API AEnemy : public ABaseCharacter 
{
	GENERATED_BODY()

public:
	AEnemy();
	//	<AActor>
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Destroyed() override;
	//	</AActor>

	// <IHitInterface>
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	// </IHitInterface>

protected:
	//	<AActor>
	virtual void BeginPlay() override;
	//	</AActor>

	//	<ABaseCharacter>
	virtual void Attack() override;
	virtual void AttackEnd() override;
	virtual void Die() override;
	virtual void HandleDamage(float DamageAmount) override;
	//	</ABaseCharacter>

	UPROPERTY(BlueprintReadOnly)
	EEnemyState EnemyState = EEnemyState::EES_NoState;

private:
	// Initialise
	AWeapon* SpawnDefaultWeapon();

	// AI Behavior
	void CheckPatrolTarget();
	void CheckCombatTarget();
	void PatrolTimerFinished();
	void HideHealthBar();
	void ShowHealthBar();
	void LoseInterest();
	void StartPatrolling();
	void ChaseTarget();
	void ClearPatrolTimer();
	void ClearAttackTimer();

	bool IsOutsideCombatRaduis();
	bool IsOutsideAttackRadius();
	bool IsInsideAttackRadius();
	bool IsChasing();
	bool IsAttacking();
	bool IsEngaged();
	bool IsDead();

	// Combat
	UFUNCTION()
	void PawnSeen(APawn* SeenPawn); // Callback for OnPawnSeen in UPawnSensingComponent
	virtual bool CanAttack() override;
	void StartAttackTimer();
	bool InTargetRange(AActor* Target, double Radius);
	
	AActor* ChoosePatrolTarget();

	FTimerHandle AttackTimer;

	UPROPERTY(EditAnywhere, Category = Combat)
	float AttackMin = 0.5f;

	UPROPERTY(EditAnywhere, Category = Combat)
	float AttackMax = 1.f;

	UPROPERTY(EditAnywhere, Category = Combat)
	float DeathLifeSpan = 10.f;

	UPROPERTY(VisibleAnywhere)
	class UPawnSensingComponent* PawnSensing;
	
	UPROPERTY(EditAnywhere)
	double CombatRadius = 1000.f;
	
	UPROPERTY(EditAnywhere)
	double AttackRadius = 150.f;
	
	UPROPERTY(EditAnywhere)
	double PatrolRadius = 200.f;

	// Components
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AWeapon> WeaponClass;
	
	UPROPERTY(VisibleAnywhere)
	class UHealthBarComponent* HealthBarWidget;

	// Navigation
	FTimerHandle PatrolTimer;

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	AActor* PatrolTarget;

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	TArray<AActor*> PatrolTargets;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float PatrolWaitMin = 5.f;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float PatrolWaitMax = 10.f;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	double RunSpeed = 250.f;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	double WalkSpeed = 125.f;

};
