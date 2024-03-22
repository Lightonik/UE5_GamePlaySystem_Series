// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterTypes.h"

#include "BarbAnimInstance.generated.h"  

/**
 * 
 */
UCLASS()
class GOBLIN1_API UBarbAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly)
	class ABarbCharacter* BarbCharacter;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	class UCharacterMovementComponent* BarbCharacterMovement;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	float GroundSpeed;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool IsFalling;

	UPROPERTY(BlueprintReadOnly, Category = "Movement | State")
	ECharacterState CharacterState; 

	UPROPERTY(BlueprintReadOnly, Category = "Movement | State")
	EActionsState ActionState;

	UPROPERTY(BlueprintReadOnly, Category = "Movement | State")
	TEnumAsByte<EDeathPose> DeathPose;
};    
