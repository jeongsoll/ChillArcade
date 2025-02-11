// Fill out your copyright notice in the Description page of Project Settings.


#include "SpaceShipRide.h"


// Sets default values
ASpaceShipRide::ASpaceShipRide()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASpaceShipRide::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpaceShipRide::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

