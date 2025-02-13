// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWaterBalloon.h"
#include "ArrLocation.h"
#include "LogUtils.h"
#include "SendArrInfoManagerComponent.h"
#include "MapGen.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABaseWaterBalloon::ABaseWaterBalloon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> BaseMeshObject
		(TEXT("/Game/Player/Balloon/Model/sm_balloon.sm_balloon"));
	if (BaseMeshObject.Succeeded()) {
		Mesh->SetStaticMesh(BaseMeshObject.Object);
	}

	SendArrComponent = CreateDefaultSubobject<USendArrInfoManagerComponent>(
		TEXT("SendArrManager")
	);

	// static ConstructorHelpers::FClassFinder<AMapGen> MapClass
	// (TEXT("/Game/Map/Blueprints/BP_Map.BP_Map_C"));
	// if (MapClass.Succeeded()) {
	// 	MapGenClass = MapClass.Class;
	// }
}

// Called when the game starts or when spawned
void ABaseWaterBalloon::BeginPlay()
{
	Super::BeginPlay();
	MapGen = Cast<AMapGen>(UGameplayStatics::GetActorOfClass(GetWorld() , AMapGen::StaticClass()));

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
	//CheckRemoveLocations(BalloonLocation);

	SendArrComponent->SendRemoveLocation(BalloonLocation);

	Destroy();
}

void ABaseWaterBalloon::CheckExplodeLocations(FArrLocation Loc)
{
	TArray<FArrLocation> Locations;
	// 설치된 위치 추가
	Locations.Add(Loc);

	int32 ExplodeRange{2};

	// 위쪽 검사
	for (int32 i{1}; i <= ExplodeRange; ++i) {
		FArrLocation UpLocation;

		if (Loc.X + i == MAP_ROW_MAX) {
			break;
		}

		UpLocation.X = Loc.X + i;
		UpLocation.Y = Loc.Y;

		if (CheckRemoveLocations(UpLocation)) {
			Locations.Add(UpLocation);
		}
		else {
			break;
		}
	}
	for (int32 i{1}; i <= ExplodeRange; ++i) {
		FArrLocation DownLocation;

		if (Loc.X - i == MAP_MIN) {
			break;
		}

		DownLocation.X = Loc.X - i;
		DownLocation.Y = Loc.Y;

		if (CheckRemoveLocations(DownLocation)) {
			Locations.Add(DownLocation);
		}
		else {
			break;
		}
	}
	for (int32 i{1}; i <= ExplodeRange; ++i) {
		FArrLocation RightLocation;

		if (Loc.Y + i == MAP_COLUMN_MAX) {
			break;
		}

		RightLocation.X = Loc.X;
		RightLocation.Y = Loc.Y + i;

		if (CheckRemoveLocations(RightLocation)) {
			Locations.Add(RightLocation);
		}
		else {
			break;
		}
	}
	for (int32 i{1}; i <= ExplodeRange; ++i) {
		FArrLocation LeftLocation;

		if (Loc.Y - i == MAP_MIN) {
			break;
		}

		LeftLocation.X = Loc.X;
		LeftLocation.Y = Loc.Y - i;

		if (CheckRemoveLocations(LeftLocation)) {
			Locations.Add(LeftLocation);
		}
		else {
			break;
		}
	}

	SendArrComponent->SendBalloonExplodeLocation(Locations);
}

bool ABaseWaterBalloon::CheckRemoveLocations(FArrLocation Loc)
{
	int32 Input{MapGen->GameMap[Loc.X][Loc.Y]};

	if (EMapType::Blocking == Input) {
		//LogUtils::Log("Blocking" , Loc.X , Loc.Y);
		return false;
	}
	if (EMapType::Pushable == Input) {
		//LogUtils::Log("Pushable : " , Loc.X , Loc.Y);
		SendArrComponent->SendRemoveLocation(Loc);
		return false;
	}
	if (EMapType::Bush == Input) {
		//LogUtils::Log("Bush : " , Loc.X , Loc.Y);
		SendArrComponent->SendRemoveLocation(Loc);
		return true;
	}
	if (EMapType::Destroyable == Input) {
		//LogUtils::Log("Destroyable : " , Loc.X , Loc.Y);
		SendArrComponent->SendRemoveLocation(Loc);
		return false;
	}
	if (EMapType::Movable == Input) {
		//LogUtils::Log("Movable : " , Loc.X , Loc.Y);
		return true;
	}

	return true;
}
