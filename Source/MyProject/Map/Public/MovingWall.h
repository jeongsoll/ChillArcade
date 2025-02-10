// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseWall.h"
#include "GameFramework/Actor.h"
#include "MovingWall.generated.h"

UCLASS()
class MYPROJECT_API AMovingWall : public ABaseWall
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMovingWall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	bool bMovingWallDestroyed = false;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual bool IsBroken() const override { return bBroken || bMovingWallDestroyed;};
	virtual void SetBroken(bool bNewState) override { bMovingWallDestroyed = bNewState;};
	
};
