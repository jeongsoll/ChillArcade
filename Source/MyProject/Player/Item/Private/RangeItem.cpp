// Fill out your copyright notice in the Description page of Project Settings.


#include "RangeItem.h"
#include "PaperSpriteComponent.h"
#include "PaperSprite.h"


// Sets default values
ARangeItem::ARangeItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	static ConstructorHelpers::FObjectFinder<UPaperSprite> RangeSpriteObject
	(TEXT("/Script/Paper2D.PaperSprite'/Game/Texture/range_Sprite.range_Sprite'"));
	if (RangeSpriteObject.Succeeded()) {
		ItemSprite->SetSprite(RangeSpriteObject.Object);
	}
}

// Called when the game starts or when spawned
void ARangeItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARangeItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

