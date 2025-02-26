// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseWall.generated.h"

UCLASS()
class MYPROJECT_API ABaseWall : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABaseWall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// 파괴되는 벽인지 아닌지 체크하는 변수
	bool bBroken = false;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// bBroken 값 가져와서 사용하기 위해 만든 Get/Set함수
	virtual bool IsBroken() const { return bBroken;};
	virtual void SetBroken(bool value) { bBroken = value;};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wall")
	class UBoxComponent* boxComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wall")
	class UStaticMeshComponent* wallMesh;
	//노드 위치 반환하는 함수
	UFUNCTION(BlueprintCallable, Category = "Tile")
	FVector2D GetBlockNumber() const;
	
};
