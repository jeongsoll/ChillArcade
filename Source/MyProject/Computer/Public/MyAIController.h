// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "MyAIController.generated.h"

UCLASS()
class MYPROJECT_API AMyAIController : public AAIController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMyAIController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// UPROPERTY()
	class AMapGen* MapGen;
	// UPROPERTY()
	class AAppleCharacter* Target;
	// UPROPERTY()
	class AComputerOne* me;
	
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float speed = 100.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FVector direciton;
	
	//탐색(플레이어 위치, 물풍선을 회피할 장소, 물풍선 설치할 위치 등 탐색)
	void FindTarget();
	//이동
	void MoveToTarget();
	//회피(물풍선을 피해 안전지대를 찾아감)
	void EvadeForBomb();
	//공격(물풍선 설치)
	void PlantBomb();
};
