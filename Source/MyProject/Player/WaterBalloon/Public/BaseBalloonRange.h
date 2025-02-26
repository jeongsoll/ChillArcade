// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArrLocation.h"
#include "GameFramework/Actor.h"
#include "BaseBalloonRange.generated.h"

UCLASS()
class MYPROJECT_API ABaseBalloonRange : public AActor {
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABaseBalloonRange();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void Initialize(const struct FArrLocation& NewLocation);
	
	UFUNCTION(BlueprintCallable)
	void CapturePLayer();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USceneComponent* Root;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* Mesh;

	UPROPERTY()
	FTimerHandle RangeTimerHandle;
	
	UFUNCTION(BlueprintCallable)
	void RangeTime();
	
	FArrLocation RangeLocation;

	UPROPERTY(EditAnywhere , BlueprintReadOnly)
	TSubclassOf<class AMapGen> MapGenClass;
	UPROPERTY(EditAnywhere , BlueprintReadOnly)
	class AMapGen* MapGen;

	UPROPERTY(EditAnywhere , BlueprintReadOnly)
	TArray<AActor*> BaseCharacters;
};
