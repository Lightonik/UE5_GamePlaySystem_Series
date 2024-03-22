// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/Action.h"

#include "ActionComponent.generated.h"

/*
USTRUCT()
struct FAction
{
	GENERATED_BODY()

	FAction(AActor* NewActionMaker, EStandartActions NewAction, AActor* NewActionTarget);
	AActor* ActionMaker;
	EStandartActions EAction;
	AActor* ActionTarget;
	float Priority = 0;

	FORCEINLINE void SetPriority(float NewPriority) { Priority = NewPriority; }
	FORCEINLINE float GetPriority() { return Priority; }
	FORCEINLINE FString GetNameOfAction() { return UEnum::GetDisplayValueAsText(EAction).ToString(); }
	FORCEINLINE EStandartActions GetAction() { return EAction; }
	FORCEINLINE AActor* GetTarget() { return ActionTarget; }

};*/

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GOBLIN1_API UActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// <UActorComponent>
	UActionComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	// </UActorComponent>

	// Action List
	void AddActionToList(AActor* ActionTarget, EStandartActions EAction);
	

protected:
	// <UActorComponent>
	virtual void BeginPlay() override;
	// </UActorComponent>

private:

	void RefreshTimer();
	UAction* NextAction();
	
	// Action List
	TArray<UAction> ActionList;
	//void RemoveFromActionList(UAction);

	UPROPERTY(VisibleAnywhere, Category = "Action List")
	TMap <EStandartActions, float> PriorityList;

	UPROPERTY(EditAnywhere, Category = "Action List")
	float DefaultActionPriority = 5.f;

	UPROPERTY(EditAnywhere, Category = "Action List")
	float DefaultIdlePriority = 1.f;


	// FTimerHandle
	float RuningTime;
	FTimerHandle ActionTimer;
	float NextActionTime = 1.f;



public:	

		
};
