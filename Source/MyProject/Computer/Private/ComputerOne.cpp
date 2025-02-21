// Fill out your copyright notice in the Description page of Project Settings.


#include "ComputerOne.h"


// Sets default values
AComputerOne::AComputerOne()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AComputerOne::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AComputerOne::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AComputerOne::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

