// Fill out your copyright notice in the Description page of Project Settings.
/*

#include "Item1.h"
#include "Goblin1/DebugMacros.h"
#include "Components/SphereComponent.h"
#include "Character/BarbCharacter.h"


// Sets default values
AItem1::AItem1()
{
 	PrimaryActorTick.bCanEverTick = true;
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Item Mesh Component"));
	RootComponent = ItemMesh;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(GetRootComponent());


}

// Called when the game starts or when spawned
void AItem1::BeginPlay()
{
	Super::BeginPlay();
	
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AItem1::OnSphereOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AItem1::OnSphereEndOverlap);
		//AddDynamic(this, &AItem1::OnSphereEndOverlap);
	

	/*
	UE_LOG(LogTemp, Warning, TEXT("Begin PLAY!"));	// ����� ������ � ���
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(1, 60.f, FColor::Emerald, FString("Item: 1-key message")); // ����� ������ �� �����
	}
	UWorld* World = GetWorld();
	FVector Location = GetActorLocation();
	FVector Forvard = GetActorForwardVector();
	DRAW_SPHERE(Location);
	DRAW_VECTOR(Location, Location + Forvard * 100.f);
	//DRAW_POINT(Location + Forvard * 100.f);
	*/
	/*
}

void AItem1::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const FString OtherActorName = OtherActor->GetName();
	/*if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 60.f, FColor::Red, OtherActorName); 
	}

	ABarbCharacter* BarbCharacter = Cast<ABarbCharacter>(OtherActor);
	if (BarbCharacter) 
	{
		BarbCharacter->SetOvelappingItem(this);

	}


}

void AItem1::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	const FString OtherActorName = OtherActor->GetName();
	/*if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(2, 60.f, FColor::Green, OtherActorName);
	}
	ABarbCharacter* BarbCharacter = Cast<ABarbCharacter>(OtherActor);
	if (BarbCharacter) 
	{
		BarbCharacter->SetOvelappingItem(nullptr);
	}

}

// Called every frame
void AItem1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	RuningTime += DeltaTime;



}

*/