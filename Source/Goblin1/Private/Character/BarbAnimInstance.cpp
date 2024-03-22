// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BarbAnimInstance.h"
#include "Character/BarbCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UBarbAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	BarbCharacter = Cast<ABarbCharacter>(TryGetPawnOwner());
	if (BarbCharacter) {
		BarbCharacterMovement = BarbCharacter->GetCharacterMovement();				
	}
}

void UBarbAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (BarbCharacterMovement) {
		GroundSpeed = UKismetMathLibrary::VSizeXY(BarbCharacterMovement->Velocity);
		IsFalling = BarbCharacterMovement->IsFalling();
		CharacterState = BarbCharacter->GetCharacterState();
		ActionState = BarbCharacter->GetActionState();
		DeathPose = BarbCharacter->GetDeathPose();
	}
}
