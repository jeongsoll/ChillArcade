// Fill out your copyright notice in the Description page of Project Settings.


#include "SpaceShipItem.h"


// Sets default values
ASpaceShipItem::ASpaceShipItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASpaceShipItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpaceShipItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

