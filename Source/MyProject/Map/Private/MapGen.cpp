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

void AMapGen::UpdateMapPlayer(FArrLocation Loc)
{
	// 플레이어 위치를 받음
	int16 playerLoc = map[Loc.X][Loc.Y] % 100;
	// 플레이어 위치 검증 어떻게? 플레이어가 그 위치에 있는지 없는지 검증
	// 플레이어가 있을 수 있는 곳 타일 = 0, 풀숲 = 4, 물풍선 = 10
	if (playerLoc == 0 || playerLoc == 4 || playerLoc == 10 || playerLoc == 14) return;
	// 받은 x,y값에 100을 더 해서 맵을 업데이트
	map[Loc.X][Loc.Y] += 100;
}

void AMapGen::UpdateMapDestroyed(FArrLocation Loc)
{
	// 파괴되는 엑터 = 풀숲(4), 부셔지는 벽(2)
	// 배열로 파괴되는 엑터가 구조체 형식으로 옴
	// 1. 물풍선 위치[x][y]를 받는다.
	// 2. 파괴되는 엑터에 위치[x][y]를 받는다.
	// 3. 다음 파괴되는 엑터에 위치[x][y]를 받는다.
	// 4. 3번과 동일하다.
	// 배열로 받기 때문에 풀어서 업데이트 해야함
	for (auto loc : Loc)
	{
		// 물풍선 위치 저장
		// 파괴된 엑터 위치 업데이트 맵 업데이트
		map[loc.X][loc.Y] = 0;
	}
}

void AMapGen::UpdateMapBalloon(FArrLocation Loc)
{
	//플레이어가 설치한 위치에 물풍선(10)으로 변경
	map[Loc.X][Loc.Y] = 10;
}

void AMapGen::UpdateMapPushed(FArrLocation Loc, FArrLocation PlayerLoc)
{
	//1초동안 플레이어의 입력이 들어오면
	//밀리는 벽을 플레이어가 바라보는 방향으로 1칸 옮긴다.
	//원래 있던 곳은 0으로 바꾼다.

	//플레이어가 밀고 있는 방향 계산
	int8 dx = Loc.X - PlayerLoc.X;
	int8 dy = Loc.Y - PlayerLoc.Y;

	//방향이 0이면 제자리에서 밀 수 없다.
	if (dx == 0 && dy == 0) return;

	// 현재 위치의 블록이 밀리는 벽(2)인지 확인
	if (map[Loc.X][Loc.Y] != 2) return;

	// 벽이 이동할 위치 계산
	int8 NewX = Loc.X + dx;
	int8 NewY = Loc.Y + dy;

	//이동할 위치가 비어 있는지 확인
	if (map[NewX][NewY] == 0)
	{
		//밀리는 쪽으로 벽 이동
		map[NewX][NewY] = 2;
		//기존 위치는 다시 타일로 변경
		map[Loc.X][Loc.Y] = 0;
	}
}

void AMapGen::UpdateMap(int8 value, int8 LocX, int8 LocY)
{
	// 타일(0번)일 때
	/*map[LocX][LocY] */
	// 부셔지는 벽(2번)일 때
	// 밀리는 벽(3번)일 때
	// 4번일 때
	// 플레이어일 때
	// 물풍선일 때
	
}
