// Fill out your copyright notice in the Description page of Project Settings.


#include "MapGen.h"
#include "ArrLocation.h"
#include "Bush.h"
#include "LogUtils.h"
#include "MovingWall.h"
#include "StrongWall.h"
#include "Tile.h"
#include "WeakWall.h"

// Sets default values
AMapGen::AMapGen()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMapGen::BeginPlay()
{
	Super::BeginPlay();
	
	LogUtils::Log("2 == map", map[5][4]);
	LogUtils::Log();
	SetGrid(gridSizeX, gridSizeY);
}

// Called every frame
void AMapGen::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
void AMapGen::SetGrid(int8 gridX, int8 gridY)
{
	UWorld* world = GetWorld();
	//그리드 반복문으로 생성
	for (int x = 0; x < gridX; ++x)
	{
		for (int y = 0; y < gridY; ++y)
		{
			UE_LOG(LogTemp, Warning, TEXT("map[%d][%d] = %d"), y, x, map[y][x]);
		
			//x,y값이 증가할 때 landSpacing을 곱해서 landSpacing만큼 떨어트리는 변수
			FVector location = GetActorLocation() + FVector(landSpacing * (gridX - 1) - (x * landSpacing), y * landSpacing, 0.0f);
			switch(map[x][y])
			{
				case 0:
					// 바닥 타일
				world->SpawnActor<ATile>(TileFactory, location, FRotator::ZeroRotator);
					break;
				case 1:
					// 안사라지는 벽(이동 불가)
					world->SpawnActor<AStrongWall>(StrongWallFactory, location, FRotator::ZeroRotator);
					break;
				case 2:
					// 사라지는 벽
					world->SpawnActor<AWeakWall>(WeakWallFactory, location, FRotator::ZeroRotator);
					break;
				case 3:
					// 밀리는 벽
					world->SpawnActor<AMovingWall>(MovingWallFactory, location, FRotator::ZeroRotator);
					break;
				case 4:
					// 풀숲
					world->SpawnActor<ABush>(BushFactory, location, FRotator::ZeroRotator);
					break;
				default:
					break;		
			}
		}
	}
}
