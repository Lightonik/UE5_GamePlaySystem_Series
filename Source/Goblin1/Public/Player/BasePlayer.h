// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BasePlayer.generated.h"

class UCapsuleComponent;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class UFloatingPawnMovement;
class UPlayerOverlay;


UCLASS()
class GOBLIN1_API ABasePlayer : public APawn
{
	GENERATED_BODY()

public:
	ABasePlayer();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	void InitOverlay();
	void SetInputSystem();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


protected:

	//Enhanced Input
	void EnhancedInputMove(const FInputActionValue& Value);
	void EnhancedInputZoom(const FInputActionValue& Value);
	void EnhancedInputRotate(const FInputActionValue& Value);


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Input")
	TSoftObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Input")
	UInputAction* InputMove;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Input")
	UInputAction* InputRotate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Input")
	UInputAction* InputZoom;

	int32 Priority = 0;

private:

	UPROPERTY(VisibleAnywhere)
	UCapsuleComponent* Capsule;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ViewCamera;

	UPROPERTY(VisibleAnywhere)
	UFloatingPawnMovement* PawnMovement;

	float MinZoomValue = 400.f;
	float MaxZoomValue = 1000.f;

	// HUD
	UPROPERTY()
	UPlayerOverlay* Overlay;


};
