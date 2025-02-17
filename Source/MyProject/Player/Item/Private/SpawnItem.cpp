// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnItem.h"

#include "ArrLocation.h"
#include "BaseCharacter.h"
#include "BaseWall.h"
#include "BubbleItem.h"
#include "CanItem.h"
#include "DevilItem.h"
#include "FluidItem.h"
#include "LogUtils.h"
#include "MapGen.h"
#include "Needle.h"
#include "RollerItem.h"
#include "SendArrInfoManagerComponent.h"
#include "ShieldItem.h"
#include "SpaceShipItem.h"
#include "SpannerItem.h"
#include "TurtleItem.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ASpawnItem::ASpawnItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASpawnItem::BeginPlay()
{
	Super::BeginPlay();
	MapGen = Cast<AMapGen>(UGameplayStatics::GetActorOfClass(GetWorld(), AMapGen::StaticClass()));
}

// Called every frame
void ASpawnItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASpawnItem::SpawnItem(struct FArrLocation Loc)
{
	// tag 검사 -> 아이템 있으면 Loc에 스폰
	if (MapGen) {
		if (MapGen->baseWalls[Loc.X][Loc.Y]->ActorHasTag("Bubble")) {
			auto BubbleActor{GetWorld()->SpawnActor<ABubbleItem>(ABubbleItem::StaticClass(), MapGen->ArrayToWorldLocation(Loc), FRotator::ZeroRotator)};
			MapGen->ItemMap[Loc.X][Loc.Y] = BubbleActor;
			MapGen->GameMap[Loc.X][Loc.Y] = EMapType::BubbleItem;
		}
		if (MapGen->baseWalls[Loc.X][Loc.Y]->ActorHasTag("Can")) {
			auto CanActor{GetWorld()->SpawnActor<ACanItem>(ACanItem::StaticClass(), MapGen->ArrayToWorldLocation(Loc), FRotator::ZeroRotator)};
			MapGen->ItemMap[Loc.X][Loc.Y] = CanActor;
			MapGen->GameMap[Loc.X][Loc.Y] = EMapType::CanItem;
		}
		if (MapGen->baseWalls[Loc.X][Loc.Y]->ActorHasTag("Devil")) {
			auto DevilActor{GetWorld()->SpawnActor<ADevilItem>(ADevilItem::StaticClass(), MapGen->ArrayToWorldLocation(Loc), FRotator::ZeroRotator)};
			MapGen->ItemMap[Loc.X][Loc.Y] = DevilActor;
			MapGen->GameMap[Loc.X][Loc.Y] = EMapType::DevilItem;
		}
		if (MapGen->baseWalls[Loc.X][Loc.Y]->ActorHasTag("Fluid")) {
			auto FluidActor{GetWorld()->SpawnActor<AFluidItem>(AFluidItem::StaticClass(), MapGen->ArrayToWorldLocation(Loc), FRotator::ZeroRotator)};
			MapGen->ItemMap[Loc.X][Loc.Y] = FluidActor;
			MapGen->GameMap[Loc.X][Loc.Y] = EMapType::FluidItem;
		}
		if (MapGen->baseWalls[Loc.X][Loc.Y]->ActorHasTag("Needle")) {
			auto NeedleActor{GetWorld()->SpawnActor<ANeedle>(ANeedle::StaticClass(), MapGen->ArrayToWorldLocation(Loc), FRotator::ZeroRotator)};
			MapGen->ItemMap[Loc.X][Loc.Y] = NeedleActor;
			MapGen->GameMap[Loc.X][Loc.Y] = EMapType::NeedleItem;
		}
		if (MapGen->baseWalls[Loc.X][Loc.Y]->ActorHasTag("Roller")) {
			auto RollerActor{GetWorld()->SpawnActor<ARollerItem>(ARollerItem::StaticClass(), MapGen->ArrayToWorldLocation(Loc), FRotator::ZeroRotator)};
			MapGen->ItemMap[Loc.X][Loc.Y] = RollerActor;
			MapGen->GameMap[Loc.X][Loc.Y] = EMapType::RollerItem;
		}
		if (MapGen->baseWalls[Loc.X][Loc.Y]->ActorHasTag("Shield")) {
			auto ShieldActor{GetWorld()->SpawnActor<AShieldItem>(AShieldItem::StaticClass(), MapGen->ArrayToWorldLocation(Loc), FRotator::ZeroRotator)};
			MapGen->ItemMap[Loc.X][Loc.Y] = ShieldActor;
			MapGen->GameMap[Loc.X][Loc.Y] = EMapType::ShieldItem;
		}
		if (MapGen->baseWalls[Loc.X][Loc.Y]->ActorHasTag("SpaceShip")) {
			auto SpaceShipActor{GetWorld()->SpawnActor<ASpaceShipItem>(ASpaceShipItem::StaticClass(), MapGen->ArrayToWorldLocation(Loc), FRotator::ZeroRotator)};
			MapGen->ItemMap[Loc.X][Loc.Y] = SpaceShipActor;
			MapGen->GameMap[Loc.X][Loc.Y] = EMapType::SpaceShipItem;
		}
		if (MapGen->baseWalls[Loc.X][Loc.Y]->ActorHasTag("Spanner")) {
			auto SpannerActor{GetWorld()->SpawnActor<ASpannerItem>(ASpannerItem::StaticClass(), MapGen->ArrayToWorldLocation(Loc), FRotator::ZeroRotator)};
			MapGen->ItemMap[Loc.X][Loc.Y] = SpannerActor;
			MapGen->GameMap[Loc.X][Loc.Y] = EMapType::SpannerItem;
		}
		if (MapGen->baseWalls[Loc.X][Loc.Y]->ActorHasTag("Turtle")) {
			auto TurtleActor{GetWorld()->SpawnActor<ATurtleItem>(ATurtleItem::StaticClass(), MapGen->ArrayToWorldLocation(Loc), FRotator::ZeroRotator)};
			MapGen->ItemMap[Loc.X][Loc.Y] = TurtleActor;
			MapGen->GameMap[Loc.X][Loc.Y] = EMapType::TurtleItem;
		}
	}
	
}

