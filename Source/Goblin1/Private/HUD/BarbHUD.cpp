// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/BarbHUD.h"
//#include "HUD/BarbOverlay.h"
#include "HUD/PlayerOverlay.h"

void ABarbHUD::BeginPlay()
{
    Super::BeginPlay();
    UWorld* World = GetWorld();
    if (World)
    {
        APlayerController* PC = World->GetFirstPlayerController();
        if (PC && PlayerOverlayClass)
        {
            Overlay = CreateWidget<UPlayerOverlay>(PC, PlayerOverlayClass);
            if (Overlay)
            {
                Overlay->AddToViewport();
            }
        }    
    }	 
}