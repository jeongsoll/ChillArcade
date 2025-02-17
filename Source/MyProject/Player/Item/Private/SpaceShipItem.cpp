// Fill out your copyright notice in the Description page of Project Settings.


#include "SpaceShipItem.h"
#include "BaseCharacter.h"
#include "SpaceShipRide.h"
#include "PaperSpriteComponent.h"
#include "PaperSprite.h"


// Sets default values
ASpaceShipItem::ASpaceShipItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UPaperSprite> SpaceShipSpriteObject
	(TEXT("/Script/Paper2D.PaperSprite'/Game/Texture/SpaceShip_Sprite.SpaceShip_Sprite'"));
	if (SpaceShipSpriteObject.Succeeded()) {
		ItemSprite->SetSprite(SpaceShipSpriteObject.Object);
	}
	
	static ConstructorHelpers::FClassFinder<ASpaceShipRide> SpaceShipClass
	(TEXT("/Game/Player/Item/BP_SpaceShipRide.BP_SpaceShipRide_C"));
	if (SpaceShipClass.Succeeded()) {
		SpaceShipRideFactory = SpaceShipClass.Class;
	}
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

void ASpaceShipItem::EquipPlayerSpaceShip(class ABaseCharacter* Character)
{
	if (!Character->CheckRide()) {
		Character->SetRide(SpaceShipRideFactory);
	}
}

