﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseWall.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

UCLASS()
class MYPROJECT_API ATile : public ABaseWall
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
};
