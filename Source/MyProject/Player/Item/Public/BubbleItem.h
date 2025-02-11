// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "BubbleItem.generated.h"

UCLASS()
class MYPROJECT_API ABubbleItem : public ABaseItem {
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABubbleItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
