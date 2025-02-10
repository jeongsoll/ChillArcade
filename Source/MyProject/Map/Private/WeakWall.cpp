// Fill out your copyright notice in the Description page of Project Settings.


#include "WeakWall.h"


// Sets default values
AWeakWall::AWeakWall()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AWeakWall::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeakWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

