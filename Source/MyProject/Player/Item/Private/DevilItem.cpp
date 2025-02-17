// Fill out your copyright notice in the Description page of Project Settings.


#include "DevilItem.h"
#include "PaperSprite.h"
#include "PaperSpriteComponent.h"

// Sets default values
ADevilItem::ADevilItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UPaperSprite> DevilSpriteObject
	(TEXT("/Script/Paper2D.PaperSprite'/Game/Texture/devil_Sprite.devil_Sprite'"));
	if (DevilSpriteObject.Succeeded()) {
		ItemSprite->SetSprite(DevilSpriteObject.Object);
	}
}

// Called when the game starts or when spawned
void ADevilItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADevilItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

