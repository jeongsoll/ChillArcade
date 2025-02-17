// Fill out your copyright notice in the Description page of Project Settings.


#include "BubbleItem.h"

#include "PaperSprite.h"
#include "PaperSpriteComponent.h"


// Sets default values
ABubbleItem::ABubbleItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UPaperSprite> BubbleSpriteObject
		(TEXT("/Script/Paper2D.PaperSprite'/Game/Texture/bubble_Sprite.bubble_Sprite'"));
	if (BubbleSpriteObject.Succeeded()) {
		ItemSprite->SetSprite(BubbleSpriteObject.Object);
	}
}

// Called when the game starts or when spawned
void ABubbleItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABubbleItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

