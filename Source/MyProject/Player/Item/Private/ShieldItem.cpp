// Fill out your copyright notice in the Description page of Project Settings.


#include "ShieldItem.h"
#include "PaperSpriteComponent.h"
#include "PaperSprite.h"


// Sets default values
AShieldItem::AShieldItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UPaperSprite> ShieldSpriteObject
	(TEXT("/Script/Paper2D.PaperSprite'/Game/Texture/shield_Sprite.shield_Sprite'"));
	if (ShieldSpriteObject.Succeeded()) {
		ItemSprite->SetSprite(ShieldSpriteObject.Object);
	}
}

// Called when the game starts or when spawned
void AShieldItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AShieldItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

