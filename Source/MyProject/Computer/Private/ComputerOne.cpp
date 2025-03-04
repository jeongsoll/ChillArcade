// Fill out your copyright notice in the Description page of Project Settings.


#include "ComputerOne.h"
#include "MapGen.h"
extern int8 GameMap[15][17]; // 맵 데이터

// Sets default values
AComputerOne::AComputerOne()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AComputerOne::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AComputerOne::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateAIState();
}

// Called to bind functionality to input
void AComputerOne::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AComputerOne::MoveToLocation(FArrLocation NewLocation)
{
	// 기존 위치를 0(빈 공간)으로 변경
	GameMap[CurrentLocation.X][CurrentLocation.Y] = 0;

	// 새로운 위치를 AI(예: 30)로 설정
	GameMap[NewLocation.X][NewLocation.Y] = 30;

	// AI 캐릭터의 현재 위치 업데이트
	CurrentLocation = NewLocation;

	// 실제 게임 캐릭터 이동
	SetActorLocation(FVector((MAP_ROW_MAX - NewLocation.X) * 100.f - 50.f, 
							 (NewLocation.Y + 1) * 100.f - 50.f, 
							 GetActorLocation().Z));
}

// 플레이어 위치 찾기
FArrLocation AComputerOne::FindPlayerLocation()
{
	for (int x = 0; x < 15; ++x)
	{
		for (int y = 0; y < 17; ++y)
		{
			if (GameMap[x][y] == 100) // 플레이어 위치
			{
				return {x, y};
			}
		}
	}
	return {-1, -1}; // 플레이어 없음
}

// BFS를 사용한 경로 탐색
TArray<FArrLocation> AComputerOne::FindPathToPlayer()
{
	TQueue<FArrLocation> Queue;
	TMap<FArrLocation, FArrLocation> CameFrom;
	TArray<FArrLocation> Path;
	FArrLocation PlayerLoc = FindPlayerLocation();

	Queue.Enqueue(CurrentLocation);
	CameFrom.Add(CurrentLocation, CurrentLocation);

	while (!Queue.IsEmpty())
	{
		FArrLocation Current;
		Queue.Dequeue(Current);

		if (Current == PlayerLoc) // 플레이어 발견
		{
			while (Current != CurrentLocation)
			{
				Path.Insert(Current, 0);
				Current = CameFrom[Current];
			}
			return Path;
		}

		// 네 방향 탐색 (상, 하, 좌, 우)
		TArray<FArrLocation> Neighbors = {
			{Current.X + 1, Current.Y}, {Current.X - 1, Current.Y},
			{Current.X, Current.Y + 1}, {Current.X, Current.Y - 1}};

		for (FArrLocation Next : Neighbors)
		{
			if (GameMap[Next.X][Next.Y] == 0 || GameMap[Next.X][Next.Y] == 100) // 이동 가능
			{
				if (!CameFrom.Contains(Next))
				{
					Queue.Enqueue(Next);
					CameFrom.Add(Next, Current);
				}
			}
		}
	}

	return {}; // 경로 없음
}

// 가장 가까운 부서지는 벽 찾기
FArrLocation AComputerOne::FindClosestBreakableWall()
{
	int32 MinDist = INT_MAX;
	FArrLocation ClosestWall = {-1, -1};

	for (int x = 0; x < 15; ++x)
	{
		for (int y = 0; y < 17; ++y)
		{
			if (GameMap[x][y] == 2) // 부서지는 벽
			{
				int32 Dist = FMath::Abs(x - CurrentLocation.X) + FMath::Abs(y - CurrentLocation.Y);
				if (Dist < MinDist)
				{
					MinDist = Dist;
					ClosestWall = {x, y};
				}
			}
		}
	}
	return ClosestWall;
}

// 물풍선 설치
void AComputerOne::PlaceBalloon()
{
	GameMap[CurrentLocation.X][CurrentLocation.Y] = 10; // 물풍선 설치
	CurrentState = EAIState::AvoidingExplosion;
}

// 안전한 대기 위치 찾기
FArrLocation AComputerOne::FindSafeLocation()
{
	for (int dx = -3; dx <= 3; ++dx)
	{
		for (int dy = -3; dy <= 3; ++dy)
		{
			int X = CurrentLocation.X + dx;
			int Y = CurrentLocation.Y + dy;

			if (X >= 0 && X < 15 && Y >= 0 && Y < 17)
			{
				if (GameMap[X][Y] == 0) // 이동 가능하고 폭발 범위 밖
				{
					bool IsSafe = true;

					for (int i = -2; i <= 2; ++i)
					{
						if ((GameMap[X + i][Y] == 10) || (GameMap[X][Y + i] == 10))
						{
							IsSafe = false;
							break;
						}
					}

					if (IsSafe)
					{
						return {X, Y};
					}
				}
			}
		}
	}
	return {-1, -1}; // 안전한 곳 없음
}

// 안전한 위치로 이동
void AComputerOne::MoveToSafeLocation(FArrLocation SafeLoc)
{
	CurrentLocation = SafeLoc;
}

// AI 상태 업데이트
void AComputerOne::UpdateAIState()
{
	
	UE_LOG(LogTemp, Warning, TEXT("%d"), CurrentState);
	switch (CurrentState)
	{
	case EAIState::Searching:
		{
			TArray<FArrLocation> Path = FindPathToPlayer();
			if (Path.Num() > 0)
			{
				MoveToLocation(Path[0]); // 첫 번째 경로로 이동
			}
			else
			{
				FArrLocation Wall = FindClosestBreakableWall();
				if (Wall.X != -1 && Wall.Y != -1)
				{
					MoveToLocation(Wall);
					CurrentState = EAIState::PlacingBalloon;
				}
			}
			break;
		}
	case EAIState::PlacingBalloon:
		PlaceBalloon();
		break;
	case EAIState::AvoidingExplosion:
		{
			FArrLocation SafeLoc = FindSafeLocation();
			if (SafeLoc.X != -1)
			{
				MoveToLocation(SafeLoc);
				CurrentState = EAIState::Waiting;
			}
			break;
		}
	case EAIState::Waiting:
		// 1초 후 다시 탐색
			FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, [this]() {
			CurrentState = EAIState::Searching;
		}, 1.0f, false);
		break;
	}
}
