// Fill out your copyright notice in the Description page of Project Settings.


#include "My_AIController.h"

#include "ComputerOne.h"
#include "MapGen.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AMy_AIController::AMy_AIController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMy_AIController::BeginPlay()
{
	Super::BeginPlay();
	
	ComOne = Cast<AComputerOne>(UGameplayStatics::GetActorOfClass(GetWorld(), AComputerOne::StaticClass()));
	map = Cast<AMapGen>(UGameplayStatics::GetActorOfClass(GetWorld(), AMapGen::StaticClass()));
}

// Called every frame
void AMy_AIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

