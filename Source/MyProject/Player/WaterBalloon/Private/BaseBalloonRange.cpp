// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseBalloonRange.h"

#include "BaseCharacter.h"
#include "BaseWaterBalloon.h"
#include "LogUtils.h"
#include "MapGen.h"
#include "MyGameInstance.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ABaseBalloonRange::ABaseBalloonRange()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> BaseMeshObject
		(TEXT("/Game/Player/Balloon/Model/sm_balloonStream.sm_balloonStream"));
	if (BaseMeshObject.Succeeded()) {
		Mesh->SetStaticMesh(BaseMeshObject.Object);
	}

	Mesh->CastShadow = false;
}

// Called when the game starts or when spawned
void ABaseBalloonRange::BeginPlay()
{
	Super::BeginPlay();

	// UMyGameInstance* GI{Cast<UMyGameInstance>(GetGameInstance())};
	// if (GI) {
	// 	Mesh = GI->SelectedBalloonRange;
	// }
	
	MapGen = Cast<AMapGen>(UGameplayStatics::GetActorOfClass(GetWorld() , AMapGen::StaticClass()));

	// 0.5초 후 물줄기 제거
	GetWorldTimerManager().SetTimer(RangeTimerHandle , this , &ABaseBalloonRange::RangeTime ,
									0.5f , false);
}

// Called every frame
void ABaseBalloonRange::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	CapturePLayer();
}

void ABaseBalloonRange::Initialize(const struct FArrLocation& NewLocation)
{
	RangeLocation = NewLocation;
	//LogUtils::Log("RangeLocation" , RangeLocation.X , RangeLocation.Y);

}

void ABaseBalloonRange::CapturePLayer()
{
	// basecharacter 상속 받은 녀석을 전부 검사하는걸로 수정 필요
	ABaseCharacter* Character = Cast<ABaseCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (Character->CheckLocation() == RangeLocation) {
		Character->Trapped();
	}
	
	if (MapGen->GameMap[RangeLocation.X][RangeLocation.Y] % 100 == 10) {
		// 이 위치 물풍선 터뜨리기
		ABaseWaterBalloon* ExtraBalloon{MapGen->waterBalloonMap[RangeLocation.X][RangeLocation.Y]};
		ExtraBalloon->ExplodeTime();
		
		//LogUtils::Log("Has Balloon" , RangeLocation.X , RangeLocation.Y);
	}
}

void ABaseBalloonRange::RangeTime()
{
	
	Destroy();
}
