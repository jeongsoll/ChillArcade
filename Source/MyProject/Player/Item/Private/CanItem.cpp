// Fill out your copyright notice in the Description page of Project Settings.


#include "CanItem.h"


// Sets default values
ACanItem::ACanItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACanItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACanItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

