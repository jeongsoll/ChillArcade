// Fill out your copyright notice in the Description page of Project Settings.


#include "SpaceShipRide.h"


// Sets default values
ASpaceShipRide::ASpaceShipRide()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset
		(TEXT("/Game/Player/Item/Model/SM_SpaceShip.SM_SpaceShip"));
	if (MeshAsset.Succeeded()) {
		Mesh->SetStaticMesh(MeshAsset.Object);
	}

	FastMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FastMesh"));
	FastMesh->SetupAttachment(RootComponent);
	FastMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> FastMeshAsset
		(TEXT("/Game/Player/Item/Model/SM_SpaceShip_Fast.SM_SpaceShip_Fast"));
	if (FastMeshAsset.Succeeded()) {
		FastMesh->SetStaticMesh(FastMeshAsset.Object);
	}

	Mesh->CastShadow = false;
	FastMesh->CastShadow = false;

}

// Called when the game starts or when spawned
void ASpaceShipRide::BeginPlay()
{
	Super::BeginPlay();

	Mesh->SetVisibility(true);
	FastMesh->SetVisibility(false);
	
}

// Called every frame
void ASpaceShipRide::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASpaceShipRide::ChangeFast()
{
	Mesh->SetVisibility(false);
	FastMesh->SetVisibility(true);
}