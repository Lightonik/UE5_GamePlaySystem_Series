// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "UObject/NoExportTypes.h"
#include "Action.generated.h"


UENUM(BlueprintType)
enum class EStandartActions : uint8
{
	ESA_Idle UMETA(DisplayName = "Idle"),	// always first

	ESA_Attack UMETA(DisplayName = "Attack"),
	ESA_MoveTo UMETA(DisplayName = "Move To"),
	ESA_PickupItem UMETA(DisplayName = "Pickup Item"),
	//ESA_EquipItem UMETA(DisplayName = "Equip Item"),




	ESA_MAX UMETA(DisplayName = "MAX")	// always last

};


//UCLASS()
class GOBLIN1_API UAction// : public UObject
{
	//GENERATED_BODY()
	
public:
	UAction(AActor* NewActionMaker, EStandartActions NewAction, AActor* NewActionTarget);
	~UAction();

private:
	AActor* ActionMaker;
	EStandartActions EAction;
	AActor* ActionTarget;
	float Priority = 0;

public:

	FORCEINLINE void SetPriority(float NewPriority) { Priority = NewPriority; }
	FORCEINLINE float GetPriority() { return Priority; }
	FORCEINLINE FString GetNameOfAction() { return UEnum::GetDisplayValueAsText(EAction).ToString(); }
	FORCEINLINE EStandartActions GetAction() { return EAction; }
	FORCEINLINE AActor* GetTarget() { return ActionTarget; }


};
