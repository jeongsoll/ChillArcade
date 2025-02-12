// Fill out your copyright notice in the Description page of Project Settings.

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

	void Initialize(const struct FArrLocation& NewLocation);
	
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
};
