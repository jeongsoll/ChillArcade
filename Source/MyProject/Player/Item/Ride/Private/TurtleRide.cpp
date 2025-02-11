// Fill out your copyright notice in the Description page of Project Settings.


#include "TurtleRide.h"


// Sets default values
ATurtleRide::ATurtleRide()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATurtleRide::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATurtleRide::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

