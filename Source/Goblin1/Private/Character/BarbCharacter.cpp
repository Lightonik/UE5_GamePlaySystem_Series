// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BarbCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimInstance.h"
#include "Items/Item.h"
#include "Items/Weapons/Weapon.h"
#include "HUD/BarbHUD.h"
#include "HUD/BarbOverlay.h"
#include "Components/StaticMeshComponent.h"
#include "Components/AttributeComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"



ABarbCharacter::ABarbCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);


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

	Tags.Add(FName("BarbCharacter"));

	InitOverlay();
	//SetInputSystem();
}

void ABarbCharacter::SetInputSystem()
{
	if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->Player))
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (!InputMappingContext.IsNull())
			{
				InputSystem->AddMappingContext(InputMappingContext.LoadSynchronous(), Priority);
			}

		}
	}
}

void ABarbCharacter::InitOverlay()
{/*
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController) {
		ABarbHUD* HUD = Cast<ABarbHUD>(PlayerController->GetHUD());
		if (HUD) {
			Overlay = HUD->GetOverlay();
			if (Overlay && Attributes) {
				Overlay->SetHealthBarPercent(Attributes->GetHealthPercent());
				Overlay->SetStaminaBarPercent(1.f);
				Overlay->SetGold(0);
				Overlay->SetSouls(0);
			}
		}
	}*/
}


// Move and lookup


void ABarbCharacter::EnhancedInputMove(const FInputActionValue& Value)
{
	/*void AFooBar::SomeCallbackFunc(const FInputActionInstance& Instance)
    {
        // Get the value of the Input Action for whatever type you want here...
        FVector VectorValue = Instance.GetValue().Get<FVector>();*/

	if (ActionsState != EActionsState::ECS_Unoccupied) return;
	if (Controller) {
		const FVector2D MoveVector = Value.Get<FVector2D>();
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);
		FVector Direction;
		//AddMovementInput(YawRotation, MoveVector.X);

		if (MoveVector.X != 0)
		{
			Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			AddMovementInput(Direction, MoveVector.X);
		}
		if (MoveVector.Y != 0)
		{
			Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
			AddMovementInput(Direction, MoveVector.Y);
		}
	}
}

void ABarbCharacter::EnhancedInputLook(const FInputActionValue& Value)
{
	const FVector2D LookVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookVector.X);
	AddControllerPitchInput(-(LookVector.Y));
}

void ABarbCharacter::EquipKeyPressed()
{
	AWeapon* OvelappingWeapon = Cast<AWeapon>(OverlappingItem);
	if (OvelappingWeapon && !EquippedWeapon) // TODO: �������� ������ �� �����
		EquipWeapon(OvelappingWeapon);		
	else 
	{
		if (CanDisarm()) 
			Disarm();

		else if (CanArm()) 
			Arm();
	}
}

void ABarbCharacter::ActionKeyPressed()
{

	//AItem* ActionItem = OverlappingItem;
	//PickUpOverlappingItem();
}

void ABarbCharacter::PickUpOverlappingItem(AItem* Item)
{
	if (OverlappingItem)
	{
		WearablesItems.AddUnique(OverlappingItem);
		OverlappingItem->HideItemMesh();
		OverlappingItem->DisableCollision();
		RemoveOvelappingItem(OverlappingItem);
	}
}

void ABarbCharacter::DropItem(AItem* Item)
{
	if (!Item) return;
	if (WearablesItems.Contains(Item))
	{
		WearablesItems.Remove(Item);

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

bool ABarbCharacter::IsUnoccupied()
{
	return ActionsState == EActionsState::ECS_Unoccupied;
}

void ABarbCharacter::Arm()
{
	PlayEquipMontage(FName("Equip"));
	CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
	ActionsState = EActionsState::ECS_EquippingWeapon;
}

void ABarbCharacter::Disarm()
{
	PlayEquipMontage(FName("Unequip"));
	CharacterState = ECharacterState::ECS_Unequipped;
	ActionsState = EActionsState::ECS_EquippingWeapon;

}

void ABarbCharacter::Die()
{
	Super::Die();
	ActionsState = EActionsState::ECS_Dead;
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABarbCharacter::Attack()
{
	Super::Attack();
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

void ABarbCharacter::EquipWeapon(AWeapon* Weapon)
{
	Weapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);
	CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
	EquippedWeapon = Weapon;
	OverlappingItem = nullptr;
}

void ABarbCharacter::AttachWeaponToBack()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("SpineSocket"));
	}
}

void ABarbCharacter::AttachWeaponToHand()
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

void ABarbCharacter::HitReactEnd()
{
	ActionsState = EActionsState::ECS_Unoccupied;
}



void ABarbCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float ABarbCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	HandleDamage(DamageAmount);
	SetHUDHealth();
	return DamageAmount;
}

void ABarbCharacter::SetHUDHealth()
{
	if (Overlay && Attributes)
	{
		Overlay->SetHealthBarPercent(Attributes->GetHealthPercent());
	}
}

// Bind control

void ABarbCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	SetInputSystem();

	//PlayerInputComponent->BindAxis(FName("MoveForward"), this, &ABarbCharacter::MoveForward);
	//PlayerInputComponent->BindAxis(FName("MoveRight"), this, &ABarbCharacter::MoveRight);
	//PlayerInputComponent->BindAxis(FName("Turn"), this, &ABarbCharacter::Turn);
	//PlayerInputComponent->BindAxis(FName("LookUp"), this, &ABarbCharacter::LookUp);
	//PlayerInputComponent->BindAction(FName("Jump"), IE_Pressed, this, &ABarbCharacter::Jump);

	PlayerInputComponent->BindAction(FName("Equip"), IE_Pressed, this, &ABarbCharacter::EquipKeyPressed);
	PlayerInputComponent->BindAction(FName("Action"), IE_Pressed, this, &ABarbCharacter::ActionKeyPressed);
	PlayerInputComponent->BindAction(FName("Attack"), IE_Pressed, this, &ABarbCharacter::Attack);

	//Enhanced Input
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	EnhancedInputComponent->BindAction(InputMove, ETriggerEvent::Triggered, this, &ABarbCharacter::EnhancedInputMove);
	EnhancedInputComponent->BindAction(InputLook, ETriggerEvent::Triggered, this, &ABarbCharacter::EnhancedInputLook);

}



void ABarbCharacter::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	Super::GetHit_Implementation(ImpactPoint, Hitter);
	if (Attributes && Attributes->GetHealthPercent() > 0.f)
	{
		ActionsState = EActionsState::ECS_HitReaction;
	}

	//SetWeaponCollision(ECollisionEnabled::NoCollision);

}
