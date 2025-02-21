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

	UPROPERTY()
	class AMapGen* MapGen;

	UPROPERTY()
	class AComputerOne* ComOne;

};
