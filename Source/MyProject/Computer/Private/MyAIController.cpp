// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAIController.h"

#include "AppleCharacter.h"
#include "MapGen.h"
#include "ArrLocation.h"
#include "ComputerOne.h"
#include "Tile.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMyAIController::AMyAIController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMyAIController::BeginPlay()
{
	Super::BeginPlay();

	MapGen = Cast<AMapGen>(UGameplayStatics::GetActorOfClass(GetWorld(), AMapGen::StaticClass()));
	Target = Cast<AAppleCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	me = Cast<AComputerOne>(GetOwner());
	
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AMyAIController::FindTarget, 2, false);
}

// Called every frame
void AMyAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}

//탐색(플레이어 위치, 물풍선을 회피할 장소, 물풍선 설치할 위치 등 탐색)
void AMyAIController::FindTarget()
{
	MapGen->PathFinding(MapGen->StartTileTemp, MapGen->TargetTileTemp);
}
//이동
void AMyAIController::MoveToTarget()
{
	
	//1. 내 위치에서 타겟 위치로 이동
	//1.1 필요속성 : 자신, 속도, 방향
	FVector p0 = me->GetActorLocation();
	direciton = me->GetActorForwardVector();
	me->SetActorLocation(p0 + direciton*speed * GetWorld()->GetDeltaSeconds());
	// me->AddActorLocalOffset(p0 + direciton * speed * GetWorld()->GetDeltaSeconds());
	//2. 이동된 위치를 향해 AI가 이동
	//3. 이동은 AddActorLocalOffset를 사용
	
	
}
//회피(물풍선을 피해 안전지대를 찾아감)
void AMyAIController::EvadeForBomb()
{
	
}
//공격(물풍선 설치)
void AMyAIController::PlantBomb()
{
	
}
