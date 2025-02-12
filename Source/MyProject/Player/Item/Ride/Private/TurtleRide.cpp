// Fill out your copyright notice in the Description page of Project Settings.


#include "TurtleRide.h"


// Sets default values
ATurtleRide::ATurtleRide()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset
		(TEXT("/Game/Player/Item/Model/SM_Car.SM_Car"));
	if (MeshAsset.Succeeded()) {
		Mesh->SetStaticMesh(MeshAsset.Object);
	}

	FastMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FastMesh"));
	FastMesh->SetupAttachment(RootComponent);
	FastMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> FastMeshAsset
		(TEXT("/Game/Player/Item/Model/SM_Car_Fast.SM_Car_Fast"));
	if (FastMeshAsset.Succeeded()) {
		FastMesh->SetStaticMesh(FastMeshAsset.Object);
	}
}

// Called when the game starts or when spawned
void ATurtleRide::BeginPlay()
{
	Super::BeginPlay();

	Mesh->SetVisibility(true);
	FastMesh->SetVisibility(false);
}

// Called every frame
void ATurtleRide::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATurtleRide::ChangeFast()
{
	Mesh->SetVisibility(false);
	FastMesh->SetVisibility(true);
}
