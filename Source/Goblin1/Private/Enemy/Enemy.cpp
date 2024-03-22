

#include "Enemy/Enemy.h"
#include "HUD/HealthBarComponent.h"
#include "Items/Weapons/Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/AttributeComponent.h"
#include "Components/ActionComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/PawnSensingComponent.h"

#include "Goblin1/DebugMacros.h"


AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);

	HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("Health Bar"));
	HealthBarWidget->SetupAttachment(GetRootComponent());

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Pawn Sensing"));
	PawnSensing->SightRadius = 4000.f;
	PawnSensing->SetPeripheralVisionAngle(45.f);
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	if (PawnSensing) PawnSensing->OnSeePawn.AddDynamic(this, &AEnemy::PawnSeen);

	// Initialise
	Tags.Add(FName("Enemy"));

	HideHealthBar();
	//MoveToTarget(PatrolTarget);
	AWeapon* NewWeapon = SpawnDefaultWeapon();
	if (NewWeapon) 
	{
		ActionList->AddActionToList(NewWeapon, EStandartActions::ESA_PickupItem);
		NewWeapon->AddNewToUsedBy(this);
	}
	ActionList->AddActionToList(this, EStandartActions::ESA_Idle);
}

AWeapon* AEnemy::SpawnDefaultWeapon()
{
	UWorld* World = GetWorld();
	if (World && WeaponClass)
	{
		AWeapon* DefaultWeapon = World->SpawnActor<AWeapon>(WeaponClass);
		FVector Location = GetActorLocation() + FVector(0.f, 1000.f, 100.f);
		DefaultWeapon->SetActorLocation(Location);
		return DefaultWeapon;
	}
	return nullptr;
}


void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (IsDead()) return;
	if (EnemyState > EEnemyState::EES_Patrolling)
	{
		CheckCombatTarget();
	}
	else
	{
		//CheckPatrolTarget();
	}
}

void AEnemy::CheckPatrolTarget()
{
	if (InTargetRange(PatrolTarget, PatrolRadius))
	{
		PatrolTarget = ChoosePatrolTarget();
		const float WaitTime = FMath::RandRange(PatrolWaitMin, PatrolWaitMax);
		GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemy::PatrolTimerFinished, WaitTime);
	}
}

void AEnemy::CheckCombatTarget()
{
	if (IsOutsideCombatRaduis())
	{
		ClearAttackTimer();
		LoseInterest();
		if (!IsEngaged()) 
			StartPatrolling();
	}
	else if (IsOutsideAttackRadius() && !IsChasing())   // виснит в состояни преследования, ?сделать проверку цели, кажные n секунд
	{
		ClearAttackTimer();
		if (!IsEngaged())
			ChaseTarget();
	}
	else if (CanAttack())
	{
		StartAttackTimer();
	}
}

void AEnemy::PawnSeen(APawn* SeenPawn)
{
	const bool bShouldChaseTarget =
		EnemyState != EEnemyState::EES_Dead &&
		EnemyState != EEnemyState::EES_Chasing &&
		EnemyState < EEnemyState::EES_Attacking &&
		SeenPawn->ActorHasTag(FName("BarbCharacter"));

	if (bShouldChaseTarget)
	{
		CombatTarget = SeenPawn;
		ClearPatrolTimer();
		ChaseTarget();
		//UE_LOG(LogTemp, Warning, TEXT("Should Chase Target"));
	}
	//else UE_LOG(LogTemp, Warning, TEXT("No need Chase Target"));
}

bool AEnemy::CanAttack()
{
	bool bCanAttack = 
		IsInsideAttackRadius() && 
		!IsAttacking() && 
		!IsEngaged() &&
		!IsDead();
	return bCanAttack;
}



void AEnemy::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	Super::GetHit_Implementation(ImpactPoint, Hitter);
	if(!IsDead())
		ShowHealthBar();
	ClearPatrolTimer();
	ClearAttackTimer();
	StopAttackMontage();
}


float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, 
	AController* EventInstigator, AActor* DamageCauser)
{
	HandleDamage(DamageAmount);
	CombatTarget = EventInstigator->GetPawn();

	if (IsInsideAttackRadius())
		EnemyState = EEnemyState::EES_Attacking;
	else if (IsOutsideAttackRadius())
		ChaseTarget();
	
	return DamageAmount;
}

void AEnemy::Destroyed()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->Destroy();
	}
}



void AEnemy::Attack()
{
	Super::Attack();
	if (!CombatTarget)
		return;
	EnemyState = EEnemyState::EES_Engaged;
	PlayAttackMontage();
	// TODO: turn to target

}

void AEnemy::AttackEnd()
{
	EnemyState = EEnemyState::EES_NoState;
	CheckCombatTarget();
}

void AEnemy::Die()
{
	Super::Die();

	// TODO: убрать повторную анимацию смерти
	EnemyState = EEnemyState::EES_Dead;
	//PlayDeathMontage();
	ClearAttackTimer();
	HideHealthBar();
	//GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DisableCapsule();
	SetLifeSpan(DeathLifeSpan);
	GetCharacterMovement()->bOrientRotationToMovement = false;
	
	// TODO: сделать дроп оружия
	SetWeaponCollision(ECollisionEnabled::NoCollision);
}

void AEnemy::HandleDamage(float DamageAmount)
{
	Super::HandleDamage(DamageAmount);

	if (Attributes && HealthBarWidget)
		HealthBarWidget->SetHealthBarPercent(Attributes->GetHealthPercent());
}

/*int32 AEnemy::PlayDeathMontage()
{
	const int32 Selection = Super::PlayDeathMontage();
	TEnumAsByte<EDeathPose> Pose(Selection);
	if (Pose < EDeathPose::EDS_MAX)
	{
		DeathPose = Pose;
	}
	return Selection;
}*/



bool AEnemy::InTargetRange(AActor* Target, double Radius)
{
	if (Target == nullptr) return false;
	const double DistanceToTarget = (Target->GetActorLocation() - GetActorLocation()).Size();
	//DRAW_SPHERE_SingleFrame(GetActorLocation());
	//DRAW_SPHERE_SingleFrame(Target->GetActorLocation());

	return DistanceToTarget <= Radius;
}

/*
void AEnemy::MoveToTarget(AActor* Target)
{
	if (EnemyController == nullptr || Target == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Target :("));
		return;
	}
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(Target);
	MoveRequest.SetAcceptanceRadius(60.f);
	EnemyController->MoveTo(MoveRequest);
	
	
}*/

AActor* AEnemy::ChoosePatrolTarget()
{
	//if (!PatrolTarget || !EnemyController) return;
	//if (InTargetRange(PatrolTarget, PatrolRadius) && PatrolTargets.Num() > 0 && EnemyController)
	
	TArray<AActor*> ValidTargets;
	for (AActor* Target : PatrolTargets)
	{
		if (Target != PatrolTarget)
		{
			ValidTargets.AddUnique(Target);
		}
	}
	if (PatrolTargets.Num() > 0)
	{
		const int32 Selection = FMath::RandRange(0, ValidTargets.Num() - 1);
		return ValidTargets[Selection];
	}
	return nullptr;
}


void AEnemy::PatrolTimerFinished()
{
	MoveToTarget(PatrolTarget);
}

void AEnemy::HideHealthBar()
{
	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(false);
	}
}

void AEnemy::ShowHealthBar()
{
	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(true);
	}
}

void AEnemy::LoseInterest()
{
	CombatTarget = nullptr;
	HideHealthBar();
	UE_LOG(LogTemp, Warning, TEXT("Lose Interest"));
}

void AEnemy::StartPatrolling()
{
	EnemyState = EEnemyState::EES_Patrolling;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	MoveToTarget(PatrolTarget);
	UE_LOG(LogTemp, Warning, TEXT("Patrolling!"));

}

void AEnemy::ChaseTarget()
{
	EnemyState = EEnemyState::EES_Chasing;
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	MoveToTarget(CombatTarget);
	UE_LOG(LogTemp, Warning, TEXT("Chasing!"));

}

void AEnemy::ClearPatrolTimer()
{
	GetWorldTimerManager().ClearTimer(PatrolTimer);
}

bool AEnemy::IsOutsideAttackRadius()
{
	return !InTargetRange(CombatTarget, AttackRadius);
}

void AEnemy::ClearAttackTimer()
{
	GetWorldTimerManager().ClearTimer(AttackTimer);
}

bool AEnemy::IsChasing()
{
	return EnemyState == EEnemyState::EES_Chasing;
}

bool AEnemy::IsInsideAttackRadius()
{
	return InTargetRange(CombatTarget, AttackRadius);
}

bool AEnemy::IsAttacking()
{
	return EnemyState == EEnemyState::EES_Attacking;
}

bool AEnemy::IsDead()
{
	return EnemyState == EEnemyState::EES_Dead;
}

void AEnemy::StartAttackTimer()
{
	EnemyState = EEnemyState::EES_Attacking;
	const float AttackTime = FMath::RandRange(AttackMin, AttackMax);
	GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, AttackTime);
}

bool AEnemy::IsEngaged()
{
	return EnemyState == EEnemyState::EES_Engaged;
}


bool AEnemy::IsOutsideCombatRaduis()
{
	return 	!InTargetRange(CombatTarget, CombatRadius);
}
