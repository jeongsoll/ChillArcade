﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "TurtleItem.generated.h"

UCLASS()
class MYPROJECT_API ATurtleItem : public ABaseItem {
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATurtleItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable)
	void EquipPlayerTurtle(class ABaseCharacter* Character);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ATurtleRide> TurtleRideFactory;
};
