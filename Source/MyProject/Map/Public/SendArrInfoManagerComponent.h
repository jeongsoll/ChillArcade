﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SendArrInfoManagerComponent.generated.h"

UCLASS(ClassGroup=(Custom) , meta=(BlueprintSpawnableComponent))
class MYPROJECT_API USendArrInfoManagerComponent : public UActorComponent {
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USendArrInfoManagerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime , ELevelTick TickType ,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere)
	class AMapGen* Map;
	
	UFUNCTION(BlueprintCallable)
	void SendPlayerLocation(struct FArrLocation Loc);
	UFUNCTION(BlueprintCallable)
	void SendBalloonLocation(struct FArrLocation Loc);
	UFUNCTION(BlueprintCallable)
	void SendBalloonExplodeLocation(TArray<struct FArrLocation> Loc);
	UFUNCTION(BlueprintCallable)
	void SendRemoveLocation(struct FArrLocation Loc);

	UFUNCTION(BlueprintCallable)
	void SendBalloonExplodeLocation_AI(TArray<struct FArrLocation> Loc);
	
};
