﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArrLocation.h"
#include "GameFramework/Actor.h"
#include "BaseWaterBalloon.generated.h"

UCLASS()
class MYPROJECT_API ABaseWaterBalloon : public AActor {
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABaseWaterBalloon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Initialize(const struct FArrLocation& NewLocation, class ABaseCharacter* NewPlayer);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	class USendArrInfoManagerComponent* SendArrComponent;

	UPROPERTY()
	FTimerHandle ExplodeTimerHandle;
	
	UFUNCTION(BlueprintCallable)
	void ExplodeTime();
	UFUNCTION(BlueprintCallable)
	void CheckExplodeLocations(struct FArrLocation Loc);
	UFUNCTION(BlueprintCallable)
	bool CheckRemoveLocations(struct FArrLocation Loc);
	// UFUNCTION(BlueprintCallable)
	// void CheckIsInMap(struct FArrLocation Loc);
	FArrLocation BalloonLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USceneComponent* Root;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere , BlueprintReadOnly)
	TSubclassOf<class AMapGen> MapGenClass;
	UPROPERTY(EditAnywhere , BlueprintReadOnly)
	class AMapGen* MapGen;

	UPROPERTY(EditAnywhere , BlueprintReadOnly)
	class ABaseCharacter* Player;

	UPROPERTY(EditAnywhere , BlueprintReadOnly)
	int32 ExplodeRange;

	UFUNCTION(BlueprintCallable)
	TArray<struct FArrLocation> CheckExplodeLocations_AI(struct FArrLocation Loc);
	UFUNCTION(BlueprintCallable)
	bool CheckRemoveLocations_AI(struct FArrLocation Loc);
};
