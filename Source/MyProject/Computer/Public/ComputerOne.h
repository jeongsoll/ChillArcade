// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArrLocation.h"
#include "BaseCharacter.h"
#include "ComputerOne.generated.h"

UCLASS()
class MYPROJECT_API AComputerOne : public ABaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AComputerOne();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void MoveToLocation(FArrLocation NewLocation);

	float MoveSpeed = 0.0f;
	FVector NextLocation;
	bool bIsUpdating = false;
	// 현재 AI 위치
	FArrLocation CurrentLocation;

	// 플레이어 위치 찾기
	FArrLocation FindPlayerLocation();

	// BFS 탐색
	TArray<FArrLocation> FindPathToPlayer();

	// 가장 가까운 부서지는 벽 찾기
	FArrLocation FindClosestBreakableWall();

	// 물풍선 설치
	void PlaceBalloon();

	// 안전한 대기 위치 찾기
	FArrLocation FindSafeLocation();

	// 안전한 위치로 이동
	void MoveToSafeLocation(FArrLocation SafeLoc);

	// 현재 AI 상태
	enum class EAIState { Searching, PlacingBalloon, AvoidingExplosion, Waiting };
	EAIState CurrentState = EAIState::Searching;

	// 상태 업데이트
	void UpdateAIState();
};
