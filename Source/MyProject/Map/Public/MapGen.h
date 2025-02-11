// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MapGen.generated.h"

UCLASS()
class MYPROJECT_API AMapGen : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMapGen();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 그리드 x축, y축 지정할 변수
	int8 gridSizeX = 15;
	int8 gridSizeY = 17;

	// 얼마나 떨어질지 정하는 변수
	int8 landSpacing = 100;

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

	//그리드 타일 세팅
	void SetGrid(int8 gridX, int8 gridY);
	
	//1.플레이어 위치  업데이트 함수
	int16 UpdateMapPlayer(struct FArrLocation Loc);

	//2.부셔지는 벽 업데이트 함수
	int16 UpdateMapDestroyed(struct FArrLocation Loc);

	//3.물풍선 설치 업데이트 함수
	int16 UpdateMapBalloon(struct FArrLocation Loc);

	//4.밀리는 벽 업데이트 함수
	void UpdateMapPushed(struct FArrLocation Loc, struct FArrLocation PlayerLoc);
};
