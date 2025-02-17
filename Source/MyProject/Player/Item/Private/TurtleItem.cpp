// Fill out your copyright notice in the Description page of Project Settings.


#include "TurtleItem.h"

#include "BaseCharacter.h"
#include "LogUtils.h"
#include "PaperSpriteComponent.h"
#include "PaperSprite.h"
#include "TurtleRide.h"


class UPaperSprite;
// Sets default values
ATurtleItem::ATurtleItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UPaperSprite> TurtleSpriteObject
	(TEXT("/Script/Paper2D.PaperSprite'/Game/Texture/turtle_Sprite.turtle_Sprite'"));
	if (TurtleSpriteObject.Succeeded()) {
		ItemSprite->SetSprite(TurtleSpriteObject.Object);
	}
	
	static ConstructorHelpers::FClassFinder<ATurtleRide> TurtleClass
		(TEXT("/Game/Player/Item/BP_TurtleRide.BP_TurtleRide_C"));
	if (TurtleClass.Succeeded()) {
		TurtleRideFactory = TurtleClass.Class;
	}
}

// Called when the game starts or when spawned
void ATurtleItem::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATurtleItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATurtleItem::EquipPlayerTurtle(class ABaseCharacter* Character)
{
	if (!Character->CheckRide()) {
		Character->SetRide(TurtleRideFactory);
	}
}

