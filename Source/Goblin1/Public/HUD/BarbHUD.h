// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BarbHUD.generated.h"


class UPlayerOverlay;


UCLASS()
class GOBLIN1_API ABarbHUD : public AHUD
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditdefaultsOnly, Category = "HUD")
	//TSubclassOf<UBarbOverlay> BarbOverlayClass;
	TSubclassOf<UPlayerOverlay> PlayerOverlayClass;
		

	UPROPERTY()
	UPlayerOverlay* Overlay;

public:
	FORCEINLINE UPlayerOverlay* GetOverlay() { return Overlay; }

};
