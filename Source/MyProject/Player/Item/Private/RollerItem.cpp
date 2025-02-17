// Fill out your copyright notice in the Description page of Project Settings.


#include "RollerItem.h"

#include "BaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PaperSpriteComponent.h"
#include "PaperSprite.h"


// Sets default values
ARollerItem::ARollerItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UPaperSprite> RollerSpriteObject
	(TEXT("/Script/Paper2D.PaperSprite'/Game/Texture/roller_Sprite.roller_Sprite'"));
	if (RollerSpriteObject.Succeeded()) {
		ItemSprite->SetSprite(RollerSpriteObject.Object);
	}
}

// Called when the game starts or when spawned
void ARollerItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARollerItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARollerItem::AddPlayerSpeed(class ABaseCharacter* Character)
{
	if (Character) {
		Character->GetCharacterMovement()->MaxWalkSpeed *= 1.5;
	}

	DestroyItem();
}



