// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseWaterBalloon.h"
#include "OneBalloon.generated.h"

UCLASS()
class MYPROJECT_API AOneBalloon : public ABaseWaterBalloon {
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AOneBalloon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
