// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWaterBalloon.h"
#include "ArrLocation.h"
#include "LogUtils.h"
#include "SendArrInfoManagerComponent.h"


// Sets default values
ABaseWaterBalloon::ABaseWaterBalloon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SendArrComponent = CreateDefaultSubobject<USendArrInfoManagerComponent>(
		TEXT("SendArrManager")
	);
}

// Called when the game starts or when spawned
void ABaseWaterBalloon::BeginPlay()
{
	Super::BeginPlay();

	// 3초 후 폭발
	GetWorldTimerManager().SetTimer(ExplodeTimerHandle , this , &ABaseWaterBalloon::ExplodeTime ,
	                                3.f , false);
}

// Called every frame
void ABaseWaterBalloon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseWaterBalloon::Initialize(const struct FArrLocation& NewLocation)
{
	BalloonLocation = NewLocation;

	LogUtils::Log("Balloon Location" , BalloonLocation.X , BalloonLocation.Y);
}

void ABaseWaterBalloon::ExplodeTime()
{
	CheckExplodeLocations(BalloonLocation);
	CheckRemoveLocations(BalloonLocation);

	Destroy();
}

void ABaseWaterBalloon::CheckExplodeLocations(FArrLocation Loc)
{
	TArray<FArrLocation> Locations;
	// 설치된 위치 추가
	Locations.Add(Loc);

	const int32 MaxX{13} , MaxY{15};
	const int32 MinX{-1} , MinY{-1};
	const int32 ExplodeRange{2};
	
	// 위쪽 검사
	for (int32 i{1}; i <= ExplodeRange; ++i) {
		//FArrLocation InitialLocation{Loc};
		//if (CheckWall(InitialLocation)) {}
		FArrLocation UpLocation;

		if (Loc.X + i == MaxX) {
			break;
		}

		UpLocation.X = Loc.X + i;
		UpLocation.Y = Loc.Y;
		Locations.Add(UpLocation);
	}
	for (int32 i{1}; i <= ExplodeRange; ++i) {
		FArrLocation DownLocation;

		if (Loc.X - i == MinX) {
			break;
		}

		DownLocation.X = Loc.X - i;
		DownLocation.Y = Loc.Y;
		Locations.Add(DownLocation);
	}
	for (int32 i{1}; i <= ExplodeRange; ++i) {
		FArrLocation RightLocation;

		if (Loc.Y + i == MaxY) {
			break;
		}

		RightLocation.X = Loc.X;
		RightLocation.Y = Loc.Y + i;
		Locations.Add(RightLocation);
	}
	for (int32 i{1}; i <= ExplodeRange; ++i) {
		FArrLocation LeftLocation;

		if (Loc.Y - i == MinY) {
			break;
		}

		LeftLocation.X = Loc.X;
		LeftLocation.Y = Loc.Y - i;
		Locations.Add(LeftLocation);
	}

	SendArrComponent->SendBalloonExplodeLocation(Locations);
}

void ABaseWaterBalloon::CheckRemoveLocations(FArrLocation Loc)
{
	TArray<FArrLocation> Locations;

	SendArrComponent->SendRemoveLocation(Locations);
}
