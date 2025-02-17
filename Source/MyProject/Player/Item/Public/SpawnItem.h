// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnItem.generated.h"

UCLASS()
class MYPROJECT_API ASpawnItem : public AActor {
	GENERATED_BODY()	

public:
	// Sets default values for this actor's properties
	ASpawnItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION()
	void SpawnItem(struct FArrLocation Loc);
	
	UPROPERTY(EditAnywhere)
		TSubclassOf<class ABubbleItem> BubbleClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ACanItem> CanClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ADevilItem> DevilClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AFluidItem> FluidClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ANeedle> NeedleClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ARangeItem> RangeClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ARollerItem> RollerClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AShieldItem> ShieldClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ASpaceShipItem> SpaceShipClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ASpannerItem> SpannerClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ATurtleItem> TurtleClass;

	UPROPERTY(EditAnywhere , BlueprintReadOnly)
	class ABubbleItem* BubbleItem;
	
	UPROPERTY(EditAnywhere , BlueprintReadOnly)
	TSubclassOf<class AMapGen> MapGenClass;
	UPROPERTY(EditAnywhere , BlueprintReadOnly)
	class AMapGen* MapGen;

	UPROPERTY(EditAnywhere , BlueprintReadOnly)
	class ABaseCharacter* Player;
};
