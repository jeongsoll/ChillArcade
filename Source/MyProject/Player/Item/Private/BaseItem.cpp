// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseItem.h"

#include "BaseCharacter.h"
#include "LogUtils.h"
#include "PaperSpriteComponent.h"
#include "PaperSprite.h"

// Sets default values
ABaseItem::ABaseItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	ItemSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("ItemSprite"));
	ItemSprite->SetupAttachment(RootComponent);
	ItemSprite->SetRelativeLocation(FVector(0.f , 0.f , 0.f));
	ItemSprite->SetRelativeRotation(FRotator(0.f , 90.f , -90.f));
	ItemSprite->SetRelativeScale3D(FVector(2.f , 1.f , 2.f));
	ItemSprite->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ShadowSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("ShadowSprite"));
	ShadowSprite->SetupAttachment(RootComponent);
	ShadowSprite->SetRelativeLocation(FVector(-50.f , 0.f , 0.f));
	ShadowSprite->SetRelativeRotation(FRotator(0.f , 90.f , -90.f));
	ShadowSprite->SetRelativeScale3D(FVector(0.7475f , 1.f , 0.7825f));
	ShadowSprite->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	static ConstructorHelpers::FObjectFinder<UPaperSprite> ShadowSpriteObject
	(TEXT("/Script/Paper2D.PaperSprite'/Game/Texture/shadow_Sprite.shadow_Sprite'"));
	if (ShadowSpriteObject.Succeeded()) {
		ShadowSprite->SetSprite(ShadowSpriteObject.Object);
	}
}

// Called when the game starts or when spawned
void ABaseItem::BeginPlay()
{
	Super::BeginPlay();

	Player = Cast<ABaseCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	
	ItemLocation = ItemSprite->GetRelativeLocation();
}

// Called every frame
void ABaseItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float Height{static_cast<float>(ItemLocation.X + FMath::Sin(GetWorld()->TimeSeconds * 3) * 5.f)};
	ItemSprite->SetRelativeLocation(FVector(Height , ItemLocation.Y , ItemLocation.Z));

	// float ShadowScale{static_cast<float>(1.f - ((1 + FMath::Sin(GetWorld()->TimeSeconds)) * 0.5f) * 0.5f)};
	// ShadowSprite->SetRelativeScale3D(FVector(ShadowSprite->GetRelativeScale3D()* 1.01));
	//LogUtils::Log("Scale", );
}

void ABaseItem::EquipItem()
{
	Player->GetItem(this);
	DestroyItem();
}

void ABaseItem::DestroyItem()
{
	Destroy();
}
