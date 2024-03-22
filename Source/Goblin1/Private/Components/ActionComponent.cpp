// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/ActionComponent.h"
#include "Items/Item.h"
#include "Interfaces/PickupInterface.h"


/*
UAction::UAction(AActor* NewActionMaker, EStandartActions NewAction, AActor* NewActionTarget)

UAction::~UAction()
{
	//delete
	//objectInstance->
}

*/

UActionComponent::UActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UActionComponent::BeginPlay()
{
	Super::BeginPlay();

	for (int32 StandartActions = 1; StandartActions < (int32)EStandartActions::ESA_MAX; StandartActions++)
	{
		const auto Action = static_cast<EStandartActions>(StandartActions);
		PriorityList.Add(Action, DefaultActionPriority);
	}
	PriorityList.Add(EStandartActions::ESA_Idle, DefaultIdlePriority);

	GetOwner()->GetWorldTimerManager().SetTimer(ActionTimer, this, &UActionComponent::RefreshTimer, NextActionTime, true, 2.0f);
	
}

void UActionComponent::RefreshTimer()
{
	UAction* Next = NextAction();

	UE_LOG(LogTemp, Warning, TEXT("Next action: %s (Priority: %f)"),
		*Next->GetNameOfAction(),
		Next->GetPriority());

	IPickupInterface* PickupInterface = Cast<IPickupInterface>(GetOwner());
	AItem* PickupItem = Cast<AItem>(Next->GetTarget());

	switch (Next->GetAction())
	{
	/*	ESA_MoveTo	*/
	case EStandartActions::ESA_MoveTo:
		if (PickupInterface)
		{
			PickupInterface->MoveToTarget(NextAction()->GetTarget());
		}
		break;

	/*	ESA_PickupItem	*/
	case EStandartActions::ESA_PickupItem:
		if (PickupInterface && PickupItem)
		{
			if (PickupInterface->IsPickedup(PickupItem))
			{
				Next->SetPriority(-1.f);
				UE_LOG(LogTemp, Warning, TEXT("SetPriority(-1.f)"));

				//ActionList.Remove(NextAction());
				break;
			}
			PickupInterface->PickupItem(PickupItem);				
		}
		break;
	}

	

}

UAction* UActionComponent::NextAction()
{
	if (ActionList.Num() <= 0)
	{
		AddActionToList(GetOwner(), EStandartActions::ESA_Idle);
		//UE_LOG(LogTemp, Warning, TEXT("Idle"));
	}
	

	UAction* NextAction = &ActionList[0];

	for (auto& Action : ActionList)
	{
		//TODO add item priority via interface

		float RenewPriority = PriorityList[Action.GetAction()];
		if (Action.GetPriority() < 0)
			continue;

		Action.SetPriority(RenewPriority);		

		if (NextAction->GetPriority() <= Action.GetPriority())
		{
			NextAction = &Action;
			/*UE_LOG(LogTemp, Warning, TEXT("Action is: %s (Priority is: %f) - %d"),
				*NextAction->GetNameOfAction(),
				NextAction->GetPriority(),
				Count);*/

		}
	}

	return NextAction;
}


void UActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	RuningTime += DeltaTime;
}

void UActionComponent::AddActionToList(AActor* ActionTarget, EStandartActions EAction)
{
	UAction NewAction(GetOwner(), EAction, ActionTarget);
	ActionList.Add(NewAction);
	UE_LOG(LogTemp, Warning, TEXT("New action: %s (Priority: %f)"),
		*NewAction.GetNameOfAction(),
		NewAction.GetPriority());
	
}

