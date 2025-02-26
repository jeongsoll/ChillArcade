// Fill out your copyright notice in the Description page of Project Settings.


#include "MapGen.h"

#include "ArrLocation.h"
#include "BaseBalloonRange.h"
#include "BaseCharacter.h"
#include "BaseWaterBalloon.h"
#include "Bush.h"
#include "LogUtils.h"
#include "MovingWall.h"
#include "SpawnItem.h"
#include "StrongWall.h"
#include "Tile.h"
#include "WeakWall.h"
#include "Kismet/GameplayStatics.h"

class ABaseWaterBalloon;
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

	SpawnItem = GetWorld()->SpawnActor<ASpawnItem>(ASpawnItem::StaticClass());

	UGameplayStatics::GetAllActorsOfClass(
		GetWorld() ,
		ABaseCharacter::StaticClass() ,
		BaseCharacters
	);


	//Player = Cast<ABaseCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());

	//맵 초기화;
	InitializeMap();

	SetGrid(gridSizeX , gridSizeY);
}

// Called every frame
void AMapGen::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMapGen::SetGrid(int8 gridX , int8 gridY)
{
	UWorld* world = GetWorld();

	//그리드 반복문으로 생성
	for (int x = 0; x < gridX; ++x) {
		for (int y = 0; y < gridY; ++y) {
			/*UE_LOG(LogTemp, Warning, TEXT("map[%d][%d] = %d"), x, y, GameMap[x][y]);*/

			//x,y값이 증가할 때 landSpacing을 곱해서 landSpacing만큼 떨어트리는 변수
			FVector location = GetActorLocation() + FVector(landSpacing * (gridX - 1) - (x * landSpacing) ,
			                                                y * landSpacing , 0.0f);

			TileLocation[x][y] = location;
			switch (GameMap[x][y]) {
			case 0:
				// 바닥 타일
				if (auto* spawntile = world->SpawnActor<ATile>(TileFactory , location , FRotator::ZeroRotator)) {
					baseWalls[x][y] = Cast<ABaseWall>(spawntile);
				}
				break;
			case 1:
				// 안사라지는 벽(이동 불가)
				if (auto* spawnStrongWall = world->SpawnActor<AStrongWall>(
					StrongWallFactory , location , FRotator::ZeroRotator)) {
					baseWalls[x][y] = Cast<ABaseWall>(spawnStrongWall);
				}
				break;
			case 2:
				// 사라지는 벽
				if (auto* spawnWeakWall = world->SpawnActor<AWeakWall>(WeakWallFactory , location ,
				                                                       FRotator::ZeroRotator)) {
					baseWalls[x][y] = Cast<ABaseWall>(spawnWeakWall);
					TaggingWall(x , y);
				}
				break;
			default:
				break;
			}
		}
	}
}

void AMapGen::UpdateMapPlayer(struct FArrLocation Loc)
{
	// 플레이어 위치를 받음
	int16 playerLoc = GameMap[Loc.X][Loc.Y] - 100;

	// 플레이어가 있을 수 있는 곳 타일 = 0, 물풍선 = 10
	if (playerLoc == -100 || playerLoc == -90) {
		// 이전 플레이어 위치를 기억할 맵 변수
		// 이전 위치 배열 위치와 현재 위치를 비교해서 다를 때만 업데이트 
		if (beforePlayerLocX != -1 && beforePlayerLocY != -1 && (beforePlayerLocX != Loc.X || beforePlayerLocY != Loc.
			Y)) {
			if (GameMap[beforePlayerLocX][beforePlayerLocY] >= 100) {
				GameMap[beforePlayerLocX][beforePlayerLocY] -= 100; // 원래 상태로 복구 (0 또는 10)
				/*UE_LOG(LogTemp, Warning, TEXT("if문 In GameMap[%d][%d] = %d beforeGameMap[%d][%d] = %d beforePlayerLocX = %d, Loc.X = %d, beforePlayerLocY = %d, Loc.Y = %d"), Loc.X, Loc.Y, GameMap[Loc.X][Loc.Y], beforePlayerLocX, beforePlayerLocY, GameMap[beforePlayerLocX][beforePlayerLocY], beforePlayerLocX, Loc.X, beforePlayerLocY, Loc.Y);*/
			}
		}
		beforePlayerLocX = Loc.X;
		beforePlayerLocY = Loc.Y;
		GameMap[Loc.X][Loc.Y] += 100;
	}
}

void AMapGen::UpdateMapDestroyed(struct FArrLocation Loc)
{
	// 파괴되는 엑터 = 부셔지는 벽(2)
	// 배열로 파괴되는 엑터가 구조체 형식으로 옴
	// 1. 물풍선 위치[x][y]를 받는다.
	// 2. 파괴되는 엑터에 위치[x][y]를 받는다.
	// 3. 다음 파괴되는 엑터에 위치[x][y]를 받는다.
	// 4. 3번과 동일하다.

	// 물풍선 위치 저장
	// 파괴된 엑터 위치 업데이트 맵 업데이트
	if (EMapType::BubbleItem <= GameMap[Loc.X][Loc.Y] && GameMap[Loc.X][Loc.Y] <= EMapType::TurtleItem) {
		ItemMap[Loc.X][Loc.Y]->Destroy();
	}

	GameMap[Loc.X][Loc.Y] = 0;
	UpdateMap(Movable , Loc.X , Loc.Y);

	//LogUtils::Log("Destroyed", GameMap[Loc.X][Loc.Y]);
}

void AMapGen::UpdateMapBalloon(struct FArrLocation Loc)
{
	if (GameMap[Loc.X][Loc.Y] % 100 == 10)
		return;

	// 아이템 있는 곳에 스폰되면
	if (EMapType::BubbleItem <= GameMap[Loc.X][Loc.Y] && GameMap[Loc.X][Loc.Y] <= EMapType::TurtleItem) {
		GameMap[Loc.X][Loc.Y] = 0;
		ItemMap[Loc.X][Loc.Y]->Destroy();
	}

	//플레이어가 설치한 위치에 물풍선(10)으로 변경
	//그 위치 위에 물풍선 스폰
	GameMap[Loc.X][Loc.Y] += 10;

	// 물풍선 스폰
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	if (BaseCharacters.Num() != 0) {
		ABaseWaterBalloon* NewBalloon = GetWorld()->SpawnActor<ABaseWaterBalloon>(
			BalloonFactory , ArrayToWorldLocation(Loc) , FRotator::ZeroRotator);
		returnWaterBalloonLoc(NewBalloon , Loc);
		if (NewBalloon) {
			for (auto* Actor : BaseCharacters) {
				ABaseCharacter* Character = Cast<ABaseCharacter>(Actor);
				if (Loc == Character->CheckLocation()) {
					NewBalloon->Initialize(Loc , Character);
				}
			}
		}
	}
	else {
		LogUtils::Log("no player");
	}
}

void AMapGen::UpdateMap(EMapType Type , int8 LocX , int8 LocY)
{
	//스폰 시키는 함수
	switch (Type) {
	case Movable: // 0
		// tag가 있으면 아이템 스폰
		if (baseWalls[LocX][LocY]->Tags.Num() > 0) {
			SpawningItem({LocX , LocY});
		}

	// 스폰하기전에 위치에 있는 엑터를 destroy하고 스폰한다.
		baseWalls[LocX][LocY]->Destroy();
		if (auto* respawnTile = GetWorld()->SpawnActor<ATile>(TileFactory , TileLocation[LocX][LocY] ,
		                                                      FRotator::ZeroRotator)) {
			baseWalls[LocX][LocY] = Cast<ABaseWall>(respawnTile);
		}
		break;
	case Blocking: // 1
		baseWalls[LocX][LocY]->Destroy();
		if (auto* respawnStrongWall = GetWorld()->SpawnActor<AStrongWall>(
			StrongWallFactory , TileLocation[LocX][LocY] , FRotator::ZeroRotator)) {
			baseWalls[LocX][LocY] = Cast<ABaseWall>(respawnStrongWall);
		}
		break;
	case Destroyable: // 2
		baseWalls[LocX][LocY]->Destroy();
		if (auto* respawnWeakWall = GetWorld()->SpawnActor<AWeakWall>(WeakWallFactory , TileLocation[LocX][LocY] ,
		                                                              FRotator::ZeroRotator)) {
			baseWalls[LocX][LocY] = Cast<ABaseWall>(respawnWeakWall);
		}
		break;
	default:
		break;
	}
}

void AMapGen::InitializeMap()
{
	for (int i = 0; i < MAP_ROW_MAX; ++i) {
		for (int j = 0; j < MAP_COLUMN_MAX; ++j) {
			GameMap[i][j] = map[i][j];

			// 아이템 나올 수 있는 블럭 개수 세기
			if (GameMap[i][j] == 2) {
				++ItemBlockCount;
			}
		}
	}
}

FVector AMapGen::ArrayToWorldLocation(struct FArrLocation Loc)
{
	return FVector((MAP_ROW_MAX - Loc.X) * 100.f - 50.f , (Loc.Y + 1) * 100.f - 50.f , 50.0f);
}

void AMapGen::UpdateMapBalloonStream(TArray<struct FArrLocation> Loc)
{
	for (const auto& Location : Loc) {
		// 스폰 예시
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride =
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		if (BaseCharacters.Num() != 0) {
			ABaseBalloonRange* NewBalloonRange{
				GetWorld()->SpawnActor<ABaseBalloonRange>(
					BalloonStreamFactory ,
					FVector(ArrayToWorldLocation(Location).X ,
					        ArrayToWorldLocation(Location).Y ,
					        ArrayToWorldLocation(Location).Z - 30.f) ,
					FRotator::ZeroRotator
				)
			};
			if (NewBalloonRange) {
				NewBalloonRange->Initialize(Location);
			}
		}
	}
}

ABaseWaterBalloon* AMapGen::returnWaterBalloonLoc(ABaseWaterBalloon* balloon , struct FArrLocation Loc)
{
	//오브젝트 저장 맵 위치에 객체 저장
	waterBalloonMap[Loc.X][Loc.Y] = balloon;
	//객체 리턴
	return waterBalloonMap[Loc.X][Loc.Y];
}

void AMapGen::SpawningItem(struct FArrLocation Loc)
{
	SpawnItem->SpawnItem(Loc);
}

void AMapGen::TaggingWall(int32 X , int32 Y)
{
	while (true) {
		int32 ChooseTag{FMath::RandRange(0 , 12)};
		if (ChooseTag + ITEM_COUNT == EMapType::BubbleItem) {
			if (ItemCounts[ChooseTag] < ItemBlockCount * 0.1) {
				++ItemCounts[ChooseTag];
				baseWalls[X][Y]->Tags.Add(FName("Bubble"));
				break;
			}

			ItemValid[ChooseTag] = true;
		}
		else if (ChooseTag + ITEM_COUNT == EMapType::CanItem) {
			if (ItemCounts[ChooseTag] < ItemBlockCount * 0.04) {
				++ItemCounts[ChooseTag];
				baseWalls[X][Y]->Tags.Add(FName("Can"));
				break;
			}

			ItemValid[ChooseTag] = true;
		}
		else if (ChooseTag + ITEM_COUNT == EMapType::DevilItem) {
			if (ItemCounts[ChooseTag] < ItemBlockCount * 0.04) {
				++ItemCounts[ChooseTag];
				baseWalls[X][Y]->Tags.Add(FName("Devil"));
				break;
			}

			ItemValid[ChooseTag] = true;
		}
		else if (ChooseTag + ITEM_COUNT == EMapType::FluidItem) {
			if (ItemCounts[ChooseTag] < ItemBlockCount * 0.1) {
				++ItemCounts[ChooseTag];
				baseWalls[X][Y]->Tags.Add(FName("Fluid"));
				break;
			}

			ItemValid[ChooseTag] = true;
		}
		else if (ChooseTag + ITEM_COUNT == EMapType::NeedleItem) {
			if (ItemCounts[ChooseTag] < ItemBlockCount * 0.02) {
				++ItemCounts[ChooseTag];
				baseWalls[X][Y]->Tags.Add(FName("Needle"));
				break;
			}

			ItemValid[ChooseTag] = true;
		}
		else if (ChooseTag + ITEM_COUNT == EMapType::RangeItem) {
			if (ItemCounts[ChooseTag] < ItemBlockCount * 0.04) {
				++ItemCounts[ChooseTag];
				baseWalls[X][Y]->Tags.Add(FName("Range"));
				break;
			}

			ItemValid[ChooseTag] = true;
		}
		else if (ChooseTag + ITEM_COUNT == EMapType::RollerItem) {
			if (ItemCounts[ChooseTag] < ItemBlockCount * 0.07) {
				++ItemCounts[ChooseTag];
				baseWalls[X][Y]->Tags.Add(FName("Roller"));
				break;
			}

			ItemValid[ChooseTag] = true;
		}
		else if (ChooseTag + ITEM_COUNT == EMapType::ShieldItem) {
			if (ItemCounts[ChooseTag] < ItemBlockCount * 0.04) {
				++ItemCounts[ChooseTag];
				baseWalls[X][Y]->Tags.Add(FName("Shield"));
				break;
			}

			ItemValid[ChooseTag] = true;
		}
		else if (ChooseTag + ITEM_COUNT == EMapType::SpaceShipItem) {
			if (ItemCounts[ChooseTag] < ItemBlockCount * 0.05) {
				++ItemCounts[ChooseTag];
				baseWalls[X][Y]->Tags.Add(FName("SpaceShip"));
				break;
			}

			ItemValid[ChooseTag] = true;
		}
		else if (ChooseTag + ITEM_COUNT == EMapType::SpannerItem) {
			if (ItemCounts[ChooseTag] < ItemBlockCount * 0.05) {
				++ItemCounts[ChooseTag];
				baseWalls[X][Y]->Tags.Add(FName("Spanner"));
				break;
			}

			ItemValid[ChooseTag] = true;
		}
		else if (ChooseTag + ITEM_COUNT == EMapType::TurtleItem) {
			if (ItemCounts[ChooseTag] < ItemBlockCount * 0.05) {
				++ItemCounts[ChooseTag];
				baseWalls[X][Y]->Tags.Add(FName("Turtle"));
				break;
			}

			ItemValid[ChooseTag] = true;
		}
		else {
			bool isAllTrue{
				std::ranges::all_of(ItemValid ,
				                    [](const bool b) {
					                    return b;
				                    })
			};
			if (ItemCounts[ChooseTag] < ItemBlockCount * 0.4 || isAllTrue) {
				++ItemCounts[ChooseTag];
				break;
			}
		}
	}
}

void AMapGen::DestroyAllMap()
{
	for (int i = 0; i < MAP_ROW_MAX; ++i) {
		for (int j = 0; j < MAP_COLUMN_MAX; ++j) {
			if (GameMap[i][j] == 2) {
				UpdateMapDestroyed({i , j});
			}
		}
	}
}

//보류
/*void AMapGen::UpdateMapPushed(struct FArrLocation Loc, struct FArrLocation PlayerLoc)
{
	// 예외처리
	if (Loc.X < 0 || Loc.Y < 0 || Loc.X >= gridSizeX || Loc.Y >= gridSizeY) return;
	
	//1초동안 플레이어의 입력이 들어오면
	//밀리는 벽을 플레이어가 바라보는 방향으로 1칸 옮긴다.
	//원래 있던 곳은 0으로 바꾼다.

	//플레이어가 밀고 있는 방향 계산
	int8 dx = Loc.X - PlayerLoc.X;
	int8 dy = Loc.Y - PlayerLoc.Y;

	//방향이 0이면 제자리에서 밀 수 없다.
	if (dx == 0 && dy == 0) return;

	// 현재 위치의 블록이 밀리는 벽(2)인지 확인
	if (GameMap[Loc.X][Loc.Y] != 2) return;

	// 벽이 이동할 위치 계산
	int8 NewX = Loc.X + dx;
	int8 NewY = Loc.Y + dy;

	//이동할 위치가 비어 있는지 확인
	if (GameMap[NewX][NewY] == 0)
	{
		//밀리는 쪽으로 벽 이동
		GameMap[NewX][NewY] = 2;
		//기존 위치는 다시 타일로 변경
		GameMap[Loc.X][Loc.Y] = 0;
	}
}*/
