﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseItem.generated.h"

UCLASS()
class MYPROJECT_API ABaseItem : public AActor {
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABaseItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable)
	void EquipItem(ABaseCharacter* NewPlayer);
	void DestroyItem();

	UPROPERTY(EditAnywhere , BlueprintReadWrite)
	class USceneComponent* Root;
	UPROPERTY(EditAnywhere , BlueprintReadWrite)
	class UPaperSpriteComponent* ItemSprite;
	UPROPERTY(EditAnywhere , BlueprintReadWrite)
	class UPaperSpriteComponent* ShadowSprite;

	FVector ItemLocation;
	UPROPERTY()
	class ABaseCharacter* Player;
};
