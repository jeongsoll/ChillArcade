// Fill out your copyright notice in the Description page of Project Settings.


#include "RollerItem.h"

#include "BaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
ARollerItem::ARollerItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ARollerItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARollerItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARollerItem::AddPlayerSpeed(class ABaseCharacter* Player)
{
	if (Player) {
		Player->GetCharacterMovement()->MaxWalkSpeed *= 1.5;
	}

	DestroyItem();
}



