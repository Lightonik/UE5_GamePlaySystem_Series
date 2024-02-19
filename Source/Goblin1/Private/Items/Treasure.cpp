// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Treasure.h"
#include "Character/BarbCharacter.h"
#include "Kismet/GameplayStatics.h"


void ATreasure::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABarbCharacter* BarbCharacter = Cast<ABarbCharacter>(OtherActor);
	if (BarbCharacter)
	{
		if (PickupSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, PickupSound, GetActorLocation());
		}
		Destroy();
	}




}
