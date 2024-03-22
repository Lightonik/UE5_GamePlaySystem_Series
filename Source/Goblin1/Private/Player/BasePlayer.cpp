// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/BasePlayer.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/SpringArmComponent.h"
#include "HUD/BarbHUD.h"
#include "HUD/PlayerOverlay.h"




ABasePlayer::ABasePlayer()
{
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->SetCapsuleHalfHeight(20.f);
	Capsule->SetCapsuleRadius(15.f);
	SetRootComponent(Capsule);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 900.f;
	SpringArm->bUsePawnControlRotation = true;

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(SpringArm);
	//ViewCamera->bUsePawnControlRotation = true;

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	PawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("PawnMovement"));

}

void ABasePlayer::BeginPlay()
{
	Super::BeginPlay();
	
	AddControllerPitchInput(20.f);
		
}

void ABasePlayer::InitOverlay()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController) {
		ABarbHUD* HUD = Cast<ABarbHUD>(PlayerController->GetHUD());
		if (HUD) {
			Overlay = HUD->GetOverlay();
			if (Overlay) {
				//TODO: initialise overlay for player
			}
		}
	}

}

void ABasePlayer::SetInputSystem()
{
	if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->Player))
	{
		//UE_LOG(LogTemp, Warning, TEXT("LocalPlayer set"));
		if (UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			//UE_LOG(LogTemp, Warning, TEXT("InputSystem set"));
			if (!InputMappingContext.IsNull())
			{
				//UE_LOG(LogTemp, Warning, TEXT("InputMappingContext set"));
				InputSystem->AddMappingContext(InputMappingContext.LoadSynchronous(), Priority);
			}
		}
	}
}

void ABasePlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void ABasePlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	SetInputSystem();

	//Enhanced Input
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	EnhancedInputComponent->BindAction(InputMove, ETriggerEvent::Triggered, this, &ABasePlayer::EnhancedInputMove);
	EnhancedInputComponent->BindAction(InputZoom, ETriggerEvent::Triggered, this, &ABasePlayer::EnhancedInputZoom);
	EnhancedInputComponent->BindAction(InputRotate, ETriggerEvent::Triggered, this, &ABasePlayer::EnhancedInputRotate);


}

void ABasePlayer::EnhancedInputMove(const FInputActionValue& Value)
{
	//UE_LOG(LogTemp, Warning, TEXT("EnhancedInputMove WASD"));

	if (Controller) {
		const FVector2D MoveVector = Value.Get<FVector2D>();
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);
		FVector Direction;



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

void ABasePlayer::EnhancedInputZoom(const FInputActionValue& Value)
{
	float VectorValue = Value.Get<FInputActionValue::Axis1D>();
	SpringArm->TargetArmLength += VectorValue*20;
	if (SpringArm->TargetArmLength > MaxZoomValue)
		SpringArm->TargetArmLength = MaxZoomValue;
	if (SpringArm->TargetArmLength < MinZoomValue)
		SpringArm->TargetArmLength = MinZoomValue;
}

void ABasePlayer::EnhancedInputRotate(const FInputActionValue& Value)
{
	float RotateVector = Value.Get<FInputActionValue::Axis1D>() / 1.5f;
	AddControllerYawInput(RotateVector);
}


