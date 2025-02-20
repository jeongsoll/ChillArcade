// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "My_AIController.generated.h"

UCLASS()
class MYPROJECT_API AMy_AIController : public AAIController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMy_AIController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	class AComputerOne* ComOne;
	UPROPERTY()
	class AMapGen* map;

	//탐색
	//공격
	//회피
	//대기
};
