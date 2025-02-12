// Fill out your copyright notice in the Description page of Project Settings.


#include "MyProject/Player/Item/Ride/Public/BaseRide.h"


// Sets default values
ABaseRide::ABaseRide()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABaseRide::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseRide::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

