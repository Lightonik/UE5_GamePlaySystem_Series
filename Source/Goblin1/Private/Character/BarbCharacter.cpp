// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BarbCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimInstance.h"
#include "Items/Item.h"
#include "Items/Weapons/Weapon.h"
#include "Components/BoxComponent.h"



// Sets default values
ABarbCharacter::ABarbCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 300.f;

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(SpringArm);

	//AutoPossessPlayer = EAutoReceiveInput::Player0;



}

void ABarbCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}


// Move and lookup

void ABarbCharacter::MoveForward(float Value)
{
	if (ActionsState != EActionsState::ECS_Unoccupied) return;
	if (Controller && (Value != 0.f)) {
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	
	}
}

void ABarbCharacter::MoveRight(float Value)
{
	if (ActionsState != EActionsState::ECS_Unoccupied) return;
	if (Controller && (Value != 0.f)) {
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);

	}
}

void ABarbCharacter::Turn(float Value)
{
	AddControllerYawInput(Value);
}

void ABarbCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}

void ABarbCharacter::EKeyPressed()
{
	AWeapon* OvelappingWeapon = Cast<AWeapon>(OverlappingItem);
	if (OvelappingWeapon && !EquippedWeapon)
	{ 
		OvelappingWeapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);
		CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
		EquippedWeapon = OvelappingWeapon;
	}
	else 
	{
		if (CanDisarm()) 		
		{
			PlayEquipMontage(FName("Unequip"));
			CharacterState = ECharacterState::ECS_Unequipped;
			ActionsState = EActionsState::ECS_EquippingWeapon;
		}
		else if (CanArm())
		{
			PlayEquipMontage(FName("Equip"));
			CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
			ActionsState = EActionsState::ECS_EquippingWeapon;
		}

	}
}

bool ABarbCharacter::CanDisarm()
{
	return CharacterState == ECharacterState::ECS_EquippedOneHandedWeapon &&
		ActionsState == EActionsState::ECS_Unoccupied;
}

bool ABarbCharacter::CanArm()
{
	return CharacterState == ECharacterState::ECS_Unequipped &&
		ActionsState == EActionsState::ECS_Unoccupied &&
		EquippedWeapon;
}

void ABarbCharacter::Attack()
{
	if (CanAttack())
	{
		PlayAttackMontage();
		ActionsState = EActionsState::ECS_Attacking;
	}
	
}

bool ABarbCharacter::CanAttack()
{
	return ActionsState == EActionsState::ECS_Unoccupied && 
		CharacterState != ECharacterState::ECS_Unequipped;
}

// Animation 

void ABarbCharacter::PlayAttackMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && AttackMontage) {
		AnimInstance->Montage_Play(AttackMontage);
		const int32 Selection = FMath::RandRange(0, 1);
		FName SectionName = FName();
		switch (Selection) {
		case 0:
			SectionName = FName("Attack1");
			break;
		case 1:
			SectionName = FName("Attack2");
			break;
		default:
			SectionName = FName("Attack1");
			break;
		}
		AnimInstance->Montage_JumpToSection(SectionName, AttackMontage);
	}
}

void ABarbCharacter::PlayEquipMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && EquipMontage) 
	{
		AnimInstance->Montage_Play(EquipMontage);
		AnimInstance->Montage_JumpToSection(SectionName, EquipMontage);
	}
}



void ABarbCharacter::AttackEnd()
{
	ActionsState = EActionsState::ECS_Unoccupied;
}

void ABarbCharacter::Disarm()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("SpineSocket"));
	}
}

void ABarbCharacter::Arm()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("RightHandSocket"));
	}
}

void ABarbCharacter::FinishEquipping()
{
	ActionsState = EActionsState::ECS_Unoccupied;
}



void ABarbCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Bind control

void ABarbCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(FName("MoveForward"), this, &ABarbCharacter::MoveForward);
	PlayerInputComponent->BindAxis(FName("MoveRight"), this, &ABarbCharacter::MoveRight);
	PlayerInputComponent->BindAxis(FName("Turn"), this, &ABarbCharacter::Turn);
	PlayerInputComponent->BindAxis(FName("LookUp"), this, &ABarbCharacter::LookUp);

	PlayerInputComponent->BindAction(FName("Jump"), IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(FName("Equip"), IE_Pressed, this, &ABarbCharacter::EKeyPressed);
	PlayerInputComponent->BindAction(FName("Attack"), IE_Pressed, this, &ABarbCharacter::Attack);



}

void ABarbCharacter::SetWeaponCollision(ECollisionEnabled::Type CollisionEnabled)
{
	if (EquippedWeapon && EquippedWeapon->GetWeaponBox())
	{
		EquippedWeapon->GetWeaponBox()->SetCollisionEnabled(CollisionEnabled);
		EquippedWeapon->IgnoreActors.Empty();
	}
		
}
