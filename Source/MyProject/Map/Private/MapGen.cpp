// Fill out your copyright notice in the Description page of Project Settings.


#include "MapGen.h"
#include "ArrLocation.h"
#include "BaseBalloonRange.h"
#include "BaseCharacter.h"
#include "BaseWaterBalloon.h"
#include "ComputerOne.h"
#include "LogUtils.h"
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

			FArrLocation Loc;
			Loc.X = x;
			Loc.Y = y;
			
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
			case PlayerLoc:
				if (auto* spawntile = world->SpawnActor<ATile>(TileFactory , location , FRotator::ZeroRotator)) {
					baseWalls[x][y] = Cast<ABaseWall>(spawntile);
				}
				if (auto* ai = world->SpawnActor<AComputerOne>(ComOneFactory , ArrayToWorldLocation(Loc) , FRotator::ZeroRotator)) {
					AiMap[x][y] = Cast<AComputerOne>(ai);
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

FVector2D AMapGen::FindClosestBreakableWall(FVector2D Start)
{
	FVector2D closestWall(-1, -1);
	int32 minDist = INT_MAX;

	for (int x = 0; x < gridSizeX; ++x)
	{
		for (int y = 0; y < gridSizeY; ++y)
		{
			if (GameMap[x][y] == 2)
			{
				int32 dist = FMath::Abs(x - Start.X) + FMath::Abs(y - Start.Y);
				if (dist < minDist)
				{
					minDist = dist;
					closestWall = FVector2D(x, y);
				}
			}
		}
	}
	return closestWall;
}
bool AMapGen::IsSafePosition(int x, int y)
{
	if (x < 0 || x >= gridSizeX || y < 0 || y >= gridSizeY) return false;
	
	// 물풍선 터질 위치 확인(3초 후 터짐)
	for (int i = -2; i <= 2; ++i)
	{
		if (GameMap[x + i][y] == 10 || GameMap[x][y + i] == 10) return false;
	}
	return true;
}

ATile* AMapGen::GetTileFromLocation(FVector2D Location)
{
	int x = static_cast<int>(Location.X);
	int y = static_cast<int>(Location.Y);

	if ( x >= 0 && x < gridSizeX && y >= 0 && y < gridSizeY) return Cast<ATile>(baseWalls[x][y]);

	return nullptr;
}


// void AMapGen::EscapeFromBomb(FArrLocation Loc)
// {
// 	FVector2D safestLoc = FVector2D(-1, -1);
//
// 	for ( int i = 0; i < 4; ++i)
// 	{
// 		int newX = Loc.X + mDirInfo[i].X;
// 		int newY = Loc.Y + mDirInfo[i].Y;
//
// 		if (IsSafePosition(newX, newY))
// 		{
// 			safestLoc = FVector2D(newX, newY);
// 			break;
// 		}
// 	}
//
// 	if ( safestLoc != FVector2D(-1, -1))
// 	{
// 		ATile* Start = GetTileFromLocation(FVector2D(Loc.X, Loc.Y));
// 		ATile* Target = GetTileFromLocation(safestLoc);
// 		if (Start && Target) PathFinding(Start, Target);
// 		else UE_LOG(LogTemp, Warning, TEXT("No path found"));
// 	}
// }

// void AMapGen::SetRelease()
// {
// 	mOpenList.Empty();
// 	mCloseList.Empty();
// 	mFinalPathList.Empty();
// }
//
// bool AMapGen::GetArrvieTarget(const FVector2D& Current, const FVector2D& Target)
// {
// 	return Current == Target;
// }
//
// void AMapGen::ReverseArray()
// {
// 	Algo::Reverse(mFinalPathList);
// }
//
// //시작 블록 선택
// void AMapGen::SelectStartBlock(ATile* Start)
// {
// 	if (StartTile != nullptr) return;
// 	StartTile = Start;
// 	FString s = StartTile->GetName();
// 	UE_LOG(LogTemp, Warning, TEXT("StartTile = %s"), *s);
// }
// //끝 블록 선택
// void AMapGen::SelectTargetBlock(ATile* Target)
// {
// 	if (Target == nullptr) return;
// 	if (TargetTile != nullptr) return;
// 	
// 	TargetTile = Target;
// 	FString s = TargetTile->GetName();
// 	UE_LOG(LogTemp, Warning, TEXT("TargetTile = %s"), *s);
// 	if (StartTile == nullptr || TargetTile == nullptr) return;
//
// 	//경로 탐색 후 결과 Path를 받음
// 	TArray<FVector2D> mCharPath = GetPath_While(StartTile->GetBlockNumber(), TargetTile->GetBlockNumber());
//
// 	//패스를 찾지 못하면
// 	if (mCharPath.Num() <= 0)
// 	{
// 		FVector2D wallLoc = FindClosestBreakableWall(StartTile->GetBlockNumber());
// 		if (wallLoc.X != -1)
// 		{
// 			mCharPath = GetPath_While(StartTile->GetBlockNumber(), wallLoc);
// 		}
// 	}
// }
//
// TArray<FVector2D> AMapGen::GetPath_While(const FVector2D& Start, const FVector2D& Target)
// {
// 	//열린 리스트, 닫힌 리스트, 최종 경로 리스트 초기화 함수
// 	this->SetRelease();
// 	
// 	FNode_Info* startNode = mNodeArr[static_cast<int>(Start.X)][static_cast<int>(Start.Y)];
// 	// 시작 노드 부모는 nullptr
// 	startNode->Parent = nullptr; 
// 	// 시작 비용 0
// 	startNode->GCost = 0; 
// 	// 맨해튼 거리
// 	startNode->HCost = FMath::Abs(Start.X - Target.X) + FMath::Abs(Start.Y - Target.Y);
// 	
// 	mOpenList.Push(startNode);
// 	
// 	while (mOpenList.Num() > 0)
// 	{
// 		// 오픈 리스트에서 F값이 가장 작은 노드 찾기
// 		mCurrentNode = mOpenList[0];
// 		int curNodeIndex = 0;
//
// 		//오픈 리스트끼리 F값과 H값을 비교한다.
// 		for (int i = 1; i < mOpenList.Num(); ++i)
// 		{
// 			// F값이 더 작거나 F값이 같을 경우 H값이 더 작은 노드 선택
// 			if(mOpenList[i]->GetCostF() < mCurrentNode->GetCostF() ||
// 			  (mOpenList[i]->GetCostF() == mCurrentNode->GetCostF() &&
// 			   mOpenList[i]->GetCostH() < mCurrentNode->GetCostH())){
// 					mCurrentNode = mOpenList[i];
// 					curNodeIndex = i;
// 			   }
// 		}
//
// 		// 계산이 끝난 값을 오픈 리스트에서 제거.
// 		mOpenList.RemoveAt(curNodeIndex);
// 		// 그리고 닫힌 목록에 추가하여 다시 계산되지 않게 한다.
// 		mCloseList.Add(mCurrentNode); 
//
// 		//목표에 도착 했는지 확인
// 		if (GetArrvieTarget(mCurrentNode->GetCurBlock(), Target))
// 		{
// 			FNode_Info* targetNode = mCurrentNode;
// 			
// 			while (targetNode != nullptr)
// 			{
// 				mFinalPathList.Push(targetNode->GetCurBlock());
// 				targetNode = targetNode->GetParent();
// 			}
// 			
// 			//정확한 경로를 추출하기 위한 역추적 
// 			ReverseArray();
// 			return mFinalPathList;
// 		}
//
// 		// 인접 노드 탐색
// 		const int curX = mCurrentNode->GetCurBlock().X;
// 		const int curY = mCurrentNode->GetCurBlock().Y;
// 		
// 		//Diagonal up down left right
// 		for (int i = 0; i < 4; ++i)
// 		{
// 			const int nextX = curX + mDirInfo[i].X;
// 			const int nextY = curY + mDirInfo[i].Y;
//
// 			//배열 범위 확인
// 			if (nextX < 0 || nextX >= gridSizeX || nextY < 0 || nextY >= gridSizeY) continue;
//
// 			//노드 유효성 확인
// 			FNode_Info* nextNode = mNodeArr[nextX][nextY];
// 			if (nextNode == nullptr) continue;
//
// 			//이동 불가 지역 확인
// 			if (GameMap[nextX][nextY] == 1) continue;
//
// 			//이미 닫힌 목록에 있는지 확인
// 			if (mCloseList.Contains(nextNode)) continue;
//
// 			//부서지는 벽 가중치 증가
// 			int moveCost = (GameMap[nextX][nextY] == 2) ? 5 : 1;
// 			int newGCost = mCurrentNode->GCost + moveCost;
//
// 			//이미 오픈 리스트에 있고, 더 좋은 경로가 아니면 스킵
// 			bool inOpenList = mOpenList.Contains(nextNode);
// 			if (inOpenList && newGCost >= nextNode->GCost) continue;
//
// 			//새 비용 및 부모 설정
// 			nextNode->Parent = mCurrentNode;
// 			nextNode->GCost = newGCost;
// 			nextNode->HCost = FMath::Abs(nextX - Target.X) + FMath::Abs(nextY - Target.Y);
//
// 			if (!inOpenList) mOpenList.Push(nextNode);
// 		}
// 	}
//
// 	//경로를 못 찾음
// 	return mFinalPathList;
// }
//
// void AMapGen::PathFinding(ATile* Start, ATile* Target)
// {
// 	if (Start == nullptr || Target == nullptr) {
// 		UE_LOG(LogTemp, Error, TEXT("Start or Target is nullptr!"));
// 		return;
// 	}
//
// 	SelectStartBlock(Start);
// 	SelectTargetBlock(Target);
// 	
// 	for (int i = 0; i < mFinalPathList.Num(); ++i)
// 	{
// 		FString s = mFinalPathList[i].ToString();
// 		UE_LOG(LogTemp, Warning, TEXT("AMapGen::PathFinding %s"), *s);
// 	}
// 	
// }


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
// void AMapGen::OpenListAdd(int x, int y)
// {
// 	//배열 범위를 넘어가면 추가하지 않는다.
// 	if (x < 0 || x >= gridSizeX || y < 0 ||  y >= gridSizeY )
// 	{
// 		UE_LOG(LogTemp, Warning, TEXT("OpenListAdd : 배열 범위를 넘김"));
// 		return;
// 	}
// 	if (mNodeArr[x][y] == nullptr)
// 	{
// 		UE_LOG(LogTemp, Warning, TEXT("AMapGen::OpenListAdd mNodeArr 배열 널값"));
// 		return;
// 	}
// 	//장애물 확인(1: 이동 불가)
// 	if (GameMap[x][y] == 1)
// 	{
// 		UE_LOG(LogTemp, Warning, TEXT("AMapGen::OpenListAdd 장애물 1 확인 %d"), GameMap[x][y]);
// 		return;
// 	}
// 	//부서지는 벽이면 가중치 증가
// 	int moveCost = (GameMap[x][y] == 2) ? 5 : 1;
// 	
// 	FNode_Info* newNode = mNodeArr[x][y];
// 	
// 	//이미 닫힌 목록에 있으면 추가하지 않음.
// 	if (mCloseList.Contains(newNode)) return;
//
// 	//새로운 G 비용 계산
// 	int newGCost = mCurrentNode->GCost + moveCost;
//
// 	// 오픈 리스트에 있는지 확인
// 	bool inOpenList = mOpenList.Contains(newNode);
// 	
// 	//오픈 리스트에 없으면 추가
// 	if (!inOpenList || newGCost < newNode->GCost)
// 	{
// 		newNode->Parent = mCurrentNode;
// 		newNode->GCost = newGCost;
// 		newNode->HCost = FMath::Abs(x - TargetTile->GetBlockNumber().X)
// 					   + FMath::Abs(y - TargetTile->GetBlockNumber().Y);
//
// 		if (!inOpenList) mOpenList.Push(newNode);
// 	}
// }