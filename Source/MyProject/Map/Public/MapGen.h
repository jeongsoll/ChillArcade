// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArrLocation.h"
#include "GameFramework/Actor.h"
#include "MapGen.generated.h"

UCLASS()
class MYPROJECT_API AMapGen : public AActor {
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMapGen();

protected:
	// Called when the game starts or when spawned
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(EditAnywhere)
	TSubclassOf<class ATile> TileFactory;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABush> BushFactory;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AMovingWall> MovingWallFactory;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AStrongWall> StrongWallFactory;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AWeakWall> WeakWallFactory;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABaseWaterBalloon> BalloonFactory;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABaseBalloonRange> BalloonStreamFactory;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AComputerOne> ComOneFactory;

	// 그리드 x축, y축 지정할 변수
	int8 gridSizeX = 15;
	int8 gridSizeY = 17;

	// 얼마나 떨어질지 정하는 변수
	int8 landSpacing = 100;

	// 맵 변수
	int8 GameMap[15][17] = {};
	// 이전 플레이어 위치를 담을 배열 변수
	int8 beforePlayerLocX = -1;
	int8 beforePlayerLocY = -1;

	// 아이템이 나올 수 있는 벽 개수
	int32 ItemBlockCount = 0;
	
	// 각 아이템 개수 저장
	int32 ItemCounts[12];
	bool ItemValid[11];
	
	// 캐릭터 변수
	class ABaseCharacter* Player;

	//객체에 대한 맵 위치 정보를 담을 배열 변수
	FVector TileLocation[15][17];
	//객체 정보를 담을 배열 변수
	class ABaseWall* baseWalls[15][17] = {};
	//물풍선 객체 정보를 담을 배열 변수
	class ABaseWaterBalloon* waterBalloonMap[15][17] = {};
	// 아이템 정보를 담을 배열 변수
	class ABaseItem* ItemMap[15][17] = {};
	// AI 정보를 담을 배열 변수
	class AComputerOne* AiMap[15][17] = {};
	
	// 그리드 타일 세팅
	void SetGrid(int8 gridX , int8 gridY);

	//1.플레이어 위치  업데이트 함수
	void UpdateMapPlayer(struct FArrLocation Loc);

	//2.부셔지는 벽 업데이트 함수
	void UpdateMapDestroyed(struct FArrLocation Loc);

	//3.물풍선 설치 업데이트 함수
	void UpdateMapBalloon(struct FArrLocation Loc);
	
	//5. 맵 업데이트 함수
	
	void UpdateMap(EMapType Type , int8 LocX , int8 LocY);

	//6. 맵 스폰 함수
	void InitializeMap();

	//7. 배열 값으로 타일 중앙값 구하는 함수
	FVector ArrayToWorldLocation(struct FArrLocation Loc); 

	//8. 물풍선 범위 스폰 함수
	void UpdateMapBalloonStream(TArray<struct FArrLocation> Loc);

	//9. 물풍선 객체 반환 하는 함수
	ABaseWaterBalloon* returnWaterBalloonLoc(ABaseWaterBalloon* balloon, struct FArrLocation Loc);
	//4.밀리는 벽 업데이트 함수
	/*void UpdateMapPushed(struct FArrLocation Loc , struct FArrLocation PlayerLoc);*/

	// 아이템 스폰 함수
	UFUNCTION()
	void SpawningItem(struct FArrLocation Loc);
	
	UPROPERTY(EditAnywhere , BlueprintReadOnly)
	TSubclassOf<class ASpawnItem> SpawnItemClass;
	UPROPERTY(EditAnywhere , BlueprintReadOnly)
	class ASpawnItem* SpawnItem;

	// Wall tag 함수
	UFUNCTION()
	void TaggingWall(int32 X, int32 Y);
	
	UFUNCTION(BlueprintCallable)
	void DestroyAllMap();


	//시작블록 선택
	//필요 속성 : ai x, y값, 플레이어 x, y값, 그 위치에 대한 객체 정보 위치 백터값
	int8 aiX = 0;
	int8 aiY = 0;
	int8 playerX = 0;
	int8 playerY = 0;
	AComputerOne* Ai;
	ATile* StartTileTemp;
	ATile* TargetTileTemp;
	ATile* StartTile;
	ATile* TargetTile;

	// 탐색용 리스트
	TArray<FNode_Info*> mOpenList; // 우선 순위 큐처럼 사용
	TSet<FNode_Info*> mCloseList;
	TArray<FVector2D> mFinalPathList;

	FNode_Info* mCurrentNode; 
	// 경로 탐색용 노드 배열
	FNode_Info* mNodeArr[15][17];
	
	//4방향 이동 저장 배열
	const FVector2D mDirInfo[4] = {
		FVector2D(0, -1), // 위
		FVector2D(0, 1), // 오른쪽
		FVector2D(-1, 0), // 아래
		FVector2D(1, 0) // 왼쪽
	};
	// 시작 지점 선택
	void SelectStartBlock(ATile* Start);
	// 목표 지점 선택
	void SelectTargetBlock(ATile* Target);
	//*****PathFinging중 사용하는 함수들*****
	// 배열 초기화
	void SetRelease();
	// 타겟과 현재 노드 위치가 목표 지점에 도착 했는지 판단
	bool GetArrvieTarget(const FVector2D& Current, const FVector2D& Target);
	//경로를 역순으로 정렬하는 함수
	void ReverseArray();
	// 새로운 노드를 오픈 리스트에 추가하는 역할을 하는 함수
	void OpenListAdd(int x, int y);
	// 경로 탐색 실패 시 부서지는 벽 찾기
	FVector2D FindClosestBreakableWall(FVector2D Start);
	
	// 경로 탐색 하는 함수
	TArray<FVector2D> GetPath_While(const FVector2D& Start, const FVector2D& Target);
	// 탐색
	void PathFinding(ATile* Start, ATile* Target);

	// 물풍선 터지는 범위로부터 안전한 위치 구하는 함수
	bool IsSafePosition(int x, int y);
	ATile* GetTileFromLocation(FVector2D Location);
	void EscapeFromBomb(FArrLocation Loc);
};

