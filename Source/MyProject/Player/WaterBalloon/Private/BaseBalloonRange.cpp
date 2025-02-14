// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseBalloonRange.h"

#include "BaseCharacter.h"


// Sets default values
ABaseBalloonRange::ABaseBalloonRange()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> BaseMeshObject
		(TEXT("/Game/Player/Balloon/Model/sm_balloonStream.sm_balloonStream"));
	if (BaseMeshObject.Succeeded()) {
		Mesh->SetStaticMesh(BaseMeshObject.Object);
	}
}

// Called when the game starts or when spawned
void ABaseBalloonRange::BeginPlay()
{
	Super::BeginPlay();

	// 1초 후 물줄기 제거
	GetWorldTimerManager().SetTimer(RangeTimerHandle , this , &ABaseBalloonRange::RangeTime ,
									1.f , false);
}

// Called every frame
void ABaseBalloonRange::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseBalloonRange::Initialize(const struct FArrLocation& NewLocation)
{
	RangeLocation = NewLocation;
}

void ABaseBalloonRange::CapturePLayer()
{
	// basecharacter 상속 받은 녀석을 전부 검사하는걸로 수정 필요
	ABaseCharacter* Character = Cast<ABaseCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (Character->CheckLocation() == RangeLocation && !Character->bIsTrapped && !Character->bIsShield) {
		Character->Trapped();
	}
}

void ABaseBalloonRange::RangeTime()
{
	CapturePLayer();
	
	Destroy();
}
