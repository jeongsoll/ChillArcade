// Fill out your copyright notice in the Description page of Project Settings.


#include "CanItem.h"
#include "PaperSpriteComponent.h"
#include "PaperSprite.h"


// Sets default values
ACanItem::ACanItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	static ConstructorHelpers::FObjectFinder<UPaperSprite> CanSpriteObject
	(TEXT("/Script/Paper2D.PaperSprite'/Game/Texture/can_Sprite.can_Sprite'"));
	if (CanSpriteObject.Succeeded()) {
		ItemSprite->SetSprite(CanSpriteObject.Object);
	}
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

