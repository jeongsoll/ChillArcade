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
	int8 gridSizeX = 17;
	int8 gridSizeY = 15;

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
	
	void SetGrid(int8 gridX, int8 gridY);
};
