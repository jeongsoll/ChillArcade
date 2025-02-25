﻿
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyController.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API AMyController : public APlayerController {
	GENERATED_BODY()

public:
	AMyController();
	

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	
	void SetBalloon(const struct FInputActionValue& Value);
	void UseEatItem(const struct FInputActionValue& Value);
	void MovePlayerUp(const struct FInputActionValue& Value);
	void MovePlayerDown(const struct FInputActionValue& Value);
	void MovePlayerRight(const struct FInputActionValue& Value);
	void MovePlayerLeft(const struct FInputActionValue& Value);
	void MoveUpCompleted(const FInputActionValue& Value);
	void MoveDownCompleted(const FInputActionValue& Value);
	void MoveRightCompleted(const FInputActionValue& Value);
	void MoveLeftCompleted(const FInputActionValue& Value);
	void UseEquippedItem(const FInputActionValue& Value);
	void UpdateDirection();
	bool CheckCollision();
	void CheckItem();

public:
	UPROPERTY(EditAnywhere , BlueprintReadOnly , Category = "Input")
	class UInputMappingContext* InputMappingContext;
	
	UPROPERTY(EditAnywhere , BlueprintReadOnly , Category = "Input")
	class UInputAction* MoveUp;
	UPROPERTY(EditAnywhere , BlueprintReadOnly , Category = "Input")
	class UInputAction* MoveDown;
	UPROPERTY(EditAnywhere , BlueprintReadOnly , Category = "Input")
	class UInputAction* MoveRight;
	UPROPERTY(EditAnywhere , BlueprintReadOnly , Category = "Input")
	class UInputAction* MoveLeft;
	UPROPERTY(EditAnywhere , BlueprintReadOnly , Category = "Input")
	class UInputAction* BalloonAction;
	UPROPERTY(EditAnywhere , BlueprintReadOnly , Category = "Input")
	class UInputAction* EatItemAction;
	UPROPERTY(EditAnywhere , BlueprintReadOnly , Category = "Input")
	class UInputAction* UseEquippedItemAction;
	
	UPROPERTY(EditAnywhere , BlueprintReadOnly)
	class ABaseCharacter* ControlledPlayer;

	UPROPERTY(EditAnywhere , BlueprintReadOnly)
	FVector Direction;

	float LastUpTime = 0.f;
	float LastDownTime = 0.f;
	float LastRightTime = 0.f;
	float LastLeftTime = 0.f;
	bool bUpPressed = false;
	bool bDownPressed = false;
	bool bRightPressed = false;
	bool bLeftPressed = false;

	UPROPERTY(EditAnywhere , BlueprintReadOnly)
	TSubclassOf<class AMapGen> MapGenClass;
	UPROPERTY(EditAnywhere , BlueprintReadOnly)
	class AMapGen* MapGen;
	
};