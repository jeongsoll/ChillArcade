// Fill out your copyright notice in the Description page of Project Settings.


#include "StrongWall.h"


// Sets default values
AStrongWall::AStrongWall()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AStrongWall::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AStrongWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

