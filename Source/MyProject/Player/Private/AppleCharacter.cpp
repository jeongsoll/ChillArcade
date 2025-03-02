﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "AppleCharacter.h"


// Sets default values
AAppleCharacter::AAppleCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	static ConstructorHelpers::FObjectFinder<UMaterial> BaseMaterialObject
		(TEXT("/Game/Player/Model/m_Apple.m_Apple"));
	if (BaseMaterialObject.Succeeded()) {
		GetMesh()->SetMaterial(0, BaseMaterialObject.Object);
	}
}

// Called when the game starts or when spawned
void AAppleCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAppleCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AAppleCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

