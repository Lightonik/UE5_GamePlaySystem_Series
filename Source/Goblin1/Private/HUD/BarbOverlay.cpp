// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/BarbOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"


void UBarbOverlay::SetHealthBarPercent(float Percent)
{
    if(HealthProgressBar)
    {
        HealthProgressBar->SetPercent(Percent);
    }
}

void UBarbOverlay::SetStaminaBarPercent(float Percent)
{
    if(StaminaProgressBar)
    {
        StaminaProgressBar->SetPercent(Percent);
    }
}

void UBarbOverlay::SetGold(int32 Gold)
{
    if(GoldText)
    {
        GoldText->SetText(FText::FromString(FString::FromInt(Gold)));
    }
}

void UBarbOverlay::SetSouls(int32 Souls)
{
    if(SoulsText)
    {
        SoulsText->SetText(FText::FromString(FString::FromInt(Souls)));
    }
}
