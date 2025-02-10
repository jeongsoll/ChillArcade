// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseWall.h"
#include "GameFramework/Actor.h"
#include "StrongWall.generated.h"

UCLASS()
class MYPROJECT_API AStrongWall : public ABaseWall
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AStrongWall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	bool bStrongWallDestroyed = false;
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual bool IsBroken() const override { return bBroken || bStrongWallDestroyed;};
	virtual void SetBroken(bool bNewState) override { bStrongWallDestroyed = bNewState;};
	
};
