// Fill out your copyright notice in the Description page of Project Settings.


#include "ComputerOne.h"
#include "MapGen.h"
extern int8 GameMap[15][17]; // 맵 데이터

// Sets default values
AComputerOne::AComputerOne()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MoveSpeed = 3.f; // 이동 속도 설정
	bIsUpdating = false;
}

// Called when the game starts or when spawned
void AComputerOne::BeginPlay()
{
	Super::BeginPlay();

	NextLocation = GetActorLocation(); // 초기에 현재 위치 저장

	//일정 시간 후에 AI 상태 업데이트 시작 (즉시 실행 방지)
	FTimerHandle StartTimer;
	GetWorldTimerManager().SetTimer(StartTimer, this, &AComputerOne::UpdateAIState, 1.0f, false);
}

// Called every frame
void AComputerOne::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// AI가 이동 중일 때만 처리
	if (!bIsUpdating)
	{
		FVector CurrentPos = GetActorLocation();
		FVector TargetPos = FVector(NextLocation.X, NextLocation.Y, CurrentPos.Z);

		// 현재 위치에서 목표 위치까지 보간
		FVector NewPos = FMath::VInterpTo(CurrentPos, TargetPos, DeltaTime, MoveSpeed);
		SetActorLocation(NewPos);

		// 목표 위치 도착 후 UpdateAIState 실행 (딜레이 추가)
		if (FVector::Dist(CurrentPos, TargetPos) < 1.f)
		{
			bIsUpdating = true; // 중복 실행 방지

			FTimerHandle TimerHandle;
			GetWorldTimerManager().SetTimer(TimerHandle, [this]()
			{
				bIsUpdating = false;
				UpdateAIState();
			}, 0.5f, false);
		}
	}
}

// Called to bind functionality to input
void AComputerOne::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AComputerOne::MoveToLocation(FArrLocation NewLocation)
{
	if (NewLocation.X < 0 || NewLocation.X >= MAP_ROW_MAX || 
		NewLocation.Y < 0 || NewLocation.Y >= MAP_COLUMN_MAX)
	{
		UE_LOG(LogTemp, Warning, TEXT("잘못된 좌표 %d, %d"), NewLocation.X, NewLocation.Y);
		return;
	}

	//벽을 통과할 수 없도록 검사 (1: 안부서지는 벽, 2: 부서지는 벽)
	if (GameMap[NewLocation.X][NewLocation.Y] == 1 || GameMap[NewLocation.X][NewLocation.Y] == 2)
	{
		UE_LOG(LogTemp, Warning, TEXT("이동 불가 %d, %d"), NewLocation.X, NewLocation.Y);
		return;
	}

	// 기존 위치를 0(빈 공간)으로 변경
	GameMap[CurrentLocation.X][CurrentLocation.Y] = 0;

	// 새로운 위치를 AI(예: 30)로 설정
	GameMap[NewLocation.X][NewLocation.Y] = 30;

	// AI 캐릭터의 현재 위치 업데이트
	CurrentLocation = NewLocation;

	// 실제 게임 캐릭터 이동 (목표 위치 저장)
	NextLocation = FVector((MAP_ROW_MAX - NewLocation.X) * 100.f - 50.f, 
							 (NewLocation.Y + 1) * 100.f - 50.f, 
							 GetActorLocation().Z);
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
	GameMap[CurrentLocation.X][CurrentLocation.Y] = 10;
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
	if (bIsUpdating) return; // 중복 실행 방지

	bIsUpdating = true; // 상태 업데이트 중으로 설정
	
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
				//경로가 없을 경우 가까운 부서지는 벽 탐색
				FArrLocation Wall = FindClosestBreakableWall();
				if (Wall.X != -1 && Wall.Y != -1)
				{
					MoveToLocation(Wall);
					CurrentState = EAIState::PlacingBalloon;
				}
				else
				{
					//이동할 곳이 없을 경우, 랜덤 이동하여 대기
					MoveToLocation(FArrLocation(CurrentLocation.X + FMath::RandRange(-1, 1), 
												 CurrentLocation.Y + FMath::RandRange(-1, 1)));
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
			bIsUpdating = false;
		}, 1.0f, false);
		break;
	}

	// 상태 업데이트 후 일정 시간 대기
	FTimerHandle DelayHandle;
	GetWorldTimerManager().SetTimer(DelayHandle, [this]()
	{
		bIsUpdating = false;
	}, 0.5f, false);
}
