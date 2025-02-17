// Fill out your copyright notice in the Description page of Project Settings.


#include "Needle.h"
#include "PaperSpriteComponent.h"
#include "PaperSprite.h"


// Sets default values
ANeedle::ANeedle()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UPaperSprite> NeedleSpriteObject
	(TEXT("/Script/Paper2D.PaperSprite'/Game/Texture/needle_Sprite.needle_Sprite'"));
	if (NeedleSpriteObject.Succeeded()) {
		ItemSprite->SetSprite(NeedleSpriteObject.Object);
	}
}

// Called when the game starts or when spawned
void ANeedle::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANeedle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

