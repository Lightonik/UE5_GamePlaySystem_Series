// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BaseCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/AttributeComponent.h"
#include "Components/ActionComponent.h"
#include "Components/CapsuleComponent.h"
#include "Items/Weapons/Weapon.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"
#include "Goblin1/DebugMacros.h"

//#include "Goblin1/DebugMacros.h"




ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));
	ActionList = CreateDefaultSubobject<UActionComponent>(TEXT("Action List"));
	InitCapsule();
	ID = FMath::RandRange(0, 99);
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	CharacterController = Cast<AAIController>(GetController());	
}

void ABaseCharacter::SetOverlappingItem(AItem* Item)
{
	OverlappingItem = Item;
}

void ABaseCharacter::AddOverlappingItem(AItem* Item)
{
	OverlappingItems.AddUnique(Item);
}


void ABaseCharacter::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	if (IsAlive() && Hitter)
		DirectionalHitReact(Hitter->GetActorLocation());

	else Die();

	PlayHitSound(ImpactPoint);
	SpawnHitParticles(ImpactPoint);
	SetWeaponCollision(ECollisionEnabled::NoCollision);
}

void ABaseCharacter::EquipWeapon(AWeapon* Weapon)
{
	Weapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);
	EquippedWeapon = Weapon;
}

void ABaseCharacter::MoveToTarget(AActor* Target)
{
	if (CharacterController == nullptr || Target == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Target or Controller :("));
		return;
	}
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(Target);
	MoveRequest.SetAcceptanceRadius(60.f);
	CharacterController->MoveTo(MoveRequest);
	//UE_LOG(LogTemp, Warning, TEXT("Moving"));
	DRAW_SPHERE_COLOR(Target->GetActorLocation(), FColor::Green);
		/*
	FNavPathSharedPtr NavPath;
	CharacterController->MoveTo(MoveRequest, &NavPath);
	TArray<FNavPathPoint>& PathPoints = NavPath->GetPathPoints();
	for (auto& Point : PathPoints)
	{
		const FVector& Location = Point.Location;
		DrawDebugSphere(GetWorld(), Location, 12.f, 12, FColor::Green, false, 10.f);
	}*/
}

void ABaseCharacter::Attack()
{
	if (CombatTarget && CombatTarget->ActorHasTag(FName("Dead")))
		CombatTarget = nullptr;
}

bool ABaseCharacter::CanAttack()
{
	return false;
}

bool ABaseCharacter::IsAlive()
{
	return Attributes && Attributes->IsAlive();
}

void ABaseCharacter::AttackEnd()
{
}

void ABaseCharacter::Die()
{
	Tags.Add(FName("Dead"));
	PlayDeathMontage();
}

void ABaseCharacter::DisableCapsule()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABaseCharacter::InitCapsule()
{
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
}

int32 ABaseCharacter::PlayAttackMontage()
{	
	return PlayRandomMontageSection(AttackMontage, AttackMontageSections);
}

int32 ABaseCharacter::PlayDeathMontage()
{
	const int32 Selection = PlayRandomMontageSection(DeathMontage, DeathMontageSections);
	TEnumAsByte<EDeathPose> Pose(Selection);
	if (Pose < EDeathPose::EDS_MAX)
	{
		DeathPose = Pose;
	}

	return Selection;
}

void ABaseCharacter::PlayMontageSection(UAnimMontage* Montage, const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && Montage) {
		AnimInstance->Montage_Play(Montage);
		AnimInstance->Montage_JumpToSection(SectionName, Montage);
	}
}

int32 ABaseCharacter::PlayRandomMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionNames)
{
	if (SectionNames.Num() <= 0) return -1;
	const int32 MaxSectionIndex = SectionNames.Num() - 1;
	const int32 Selection = FMath::RandRange(0, MaxSectionIndex);
	PlayMontageSection(Montage, SectionNames[Selection]);

	return Selection;
}

void ABaseCharacter::RemoveOvelappingItem(AItem* Item)
{
	if (!Item) return;
	OverlappingItems.Remove(Item);
	if (OverlappingItems.Num() > 0)
		SetOverlappingItem(OverlappingItems[0]);
	else
		SetOverlappingItem(nullptr);
}

void ABaseCharacter::PickupItem(AItem* Item)
{
	if (!Item) return;
	if (OverlappingItems.Contains(Item))
	{
		PickUpOverlappingItem(Item);
		// TODO: fast pick up (using ::OveralingItom)
	}
	else
	{
		MoveToTarget(Item);
	}
}

void ABaseCharacter::PickUpOverlappingItem(AItem* Item)
{
	WearablesItems.AddUnique(Item);
	Item->HideItemMesh();
	Item->DisableCollision();
	RemoveOvelappingItem(Item);
}

bool ABaseCharacter::IsPickedup(AItem* Item)
{
	if (WearablesItems.Contains(Item))
		return true;
	return false;
}

void ABaseCharacter::PlayHitReactMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && HitReactMontage)
	{
		AnimInstance->Montage_Play(HitReactMontage);
		AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);
	}
}

void ABaseCharacter::DirectionalHitReact(const FVector& ImpactPoint)
{
	const FVector Forward = GetActorForwardVector();
	const FVector ImpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	const FVector ToHit = (ImpactLowered - GetActorLocation()).GetSafeNormal();

	const double CosTheta = FVector::DotProduct(Forward, ToHit);
	double Theta = FMath::Acos(CosTheta); //radians
	Theta = FMath::RadiansToDegrees(Theta);

	//if CrossProduct points down, Thera should be negative 
	const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);
	//UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + CrossProduct * 60.f, 5.f, FColor::Green, 5.f);

	if (CrossProduct.Z < 0) Theta *= -1.f;

	FName Section("FromBack");
	if (Theta >= -45.f && Theta <= 45.f)
		Section = FName("FromFront");
	else if (Theta >= -135.f && Theta < -45.f)
		Section = FName("FromLeft");
	else if (Theta < 135.f && Theta >= 45.f)
		Section = FName("FromRight");
	PlayHitReactMontage(Section);


	/*if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 15.f, FColor::Green, FString::Printf(TEXT("%f degrees"), Theta));
	}
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + Forward * 60.f, 5.f, FColor::Red, 15.f);
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + ToHit * 60.f, 5.f, FColor::Yellow, 15.f);
	*/


}

void ABaseCharacter::StopAttackMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
		AnimInstance->Montage_Stop(0.25, AttackMontage);
}

FVector ABaseCharacter::GetTranslationWarpTarget()
{
	if (CombatTarget == nullptr)
		return FVector();
	const FVector CombatTargetLocation = CombatTarget->GetActorLocation();
	const FVector Location = GetActorLocation(); 
	FVector TargetToMe = (Location - CombatTargetLocation).GetSafeNormal();
	TargetToMe *= WarpTargetDistance;

	return TargetToMe + CombatTargetLocation;
}

FVector ABaseCharacter::GetRotationWarpTarget()
{
	if (CombatTarget)
	{
		return CombatTarget->GetActorLocation();
	}
	return FVector();
}

void ABaseCharacter::PlayHitSound(const FVector& ImpactPoint)
{
	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, ImpactPoint);
	}
}

void ABaseCharacter::SpawnHitParticles(const FVector& ImpactPoint)
{
	if (HitParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticles, ImpactPoint);
	}

}

void ABaseCharacter::HandleDamage(float DamageAmount)
{
	if (Attributes)
		Attributes->ReceiveDamage(DamageAmount);
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseCharacter::SetWeaponCollision(ECollisionEnabled::Type CollisionEnabled)
{
	if (EquippedWeapon && EquippedWeapon->GetWeaponBox())
	{
		EquippedWeapon->GetWeaponBox()->SetCollisionEnabled(CollisionEnabled);
		EquippedWeapon->IgnoreActors.Empty();
	}
}

