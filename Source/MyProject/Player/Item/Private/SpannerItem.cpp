// Fill out your copyright notice in the Description page of Project Settings.


#include "SpannerItem.h"
#include "PaperSpriteComponent.h"
#include "PaperSprite.h"


// Sets default values
ASpannerItem::ASpannerItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UPaperSprite> SpannerSpriteObject
	(TEXT("/Script/Paper2D.PaperSprite'/Game/Texture/spanner_Sprite.spanner_Sprite'"));
	if (SpannerSpriteObject.Succeeded()) {
		ItemSprite->SetSprite(SpannerSpriteObject.Object);
	}
}

// Called when the game starts or when spawned
void ASpannerItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpannerItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

