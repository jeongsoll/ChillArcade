// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseBalloonRange.h"
#include "OneBalloonRange.generated.h"

UCLASS()
class MYPROJECT_API AOneBalloonRange : public ABaseBalloonRange {
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AOneBalloonRange();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;



	
};
