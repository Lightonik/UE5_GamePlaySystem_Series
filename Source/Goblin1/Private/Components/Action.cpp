// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Action.h"

UAction::UAction(AActor* NewActionMaker, EStandartActions NewAction, AActor* NewActionTarget)
{
	ActionMaker = NewActionMaker;
	EAction = NewAction;
	ActionTarget = NewActionTarget;
}

UAction::~UAction()
{
	//Priority = -1.f;
}
