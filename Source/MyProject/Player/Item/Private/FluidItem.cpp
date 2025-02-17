// Fill out your copyright notice in the Description page of Project Settings.


#include "FluidItem.h"
#include "PaperSpriteComponent.h"
#include "PaperSprite.h"


// Sets default values
AFluidItem::AFluidItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UPaperSprite> FluidSpriteObject
	(TEXT("/Script/Paper2D.PaperSprite'/Game/Texture/fluid_Sprite.fluid_Sprite'"));
	if (FluidSpriteObject.Succeeded()) {
		ItemSprite->SetSprite(FluidSpriteObject.Object);
	}
}

// Called when the game starts or when spawned
void AFluidItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFluidItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

