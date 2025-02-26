
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
	class ABaseCharacter* ControlledPlayer2;

	float LastUpTime{0.f};
	float LastDownTime{0.f};
	float LastRightTime{0.f};
	float LastLeftTime{0.f};
	bool bUpPressed{false};
	bool bDownPressed{false};
	bool bRightPressed{false};
	bool bLeftPressed{false};

	UPROPERTY(EditAnywhere , BlueprintReadOnly)
	TSubclassOf<class AMapGen> MapGenClass;
	UPROPERTY(EditAnywhere , BlueprintReadOnly)
	class AMapGen* MapGen;





////////////////////////////////////////////////////////////////


	void SetBalloon2(const struct FInputActionValue& Value);
	void UseEatItem2(const struct FInputActionValue& Value);
	void MovePlayerUp2(const struct FInputActionValue& Value);
	void MovePlayerDown2(const struct FInputActionValue& Value);
	void MovePlayerRight2(const struct FInputActionValue& Value);
	void MovePlayerLeft2(const struct FInputActionValue& Value);
	void MoveUpCompleted2(const FInputActionValue& Value);
	void MoveDownCompleted2(const FInputActionValue& Value);
	void MoveRightCompleted2(const FInputActionValue& Value);
	void MoveLeftCompleted2(const FInputActionValue& Value);
	void UseEquippedItem2(const FInputActionValue& Value);
	void UpdateDirection2();
	bool CheckCollision2();
	void CheckItem2();
	
	UPROPERTY(EditAnywhere , BlueprintReadOnly , Category = "Input")
	class UInputAction* MoveUp2;
	UPROPERTY(EditAnywhere , BlueprintReadOnly , Category = "Input")
	class UInputAction* MoveDown2;
	UPROPERTY(EditAnywhere , BlueprintReadOnly , Category = "Input")
	class UInputAction* MoveRight2;
	UPROPERTY(EditAnywhere , BlueprintReadOnly , Category = "Input")
	class UInputAction* MoveLeft2;
	UPROPERTY(EditAnywhere , BlueprintReadOnly , Category = "Input")
	class UInputAction* BalloonAction2;
	UPROPERTY(EditAnywhere , BlueprintReadOnly , Category = "Input")
	class UInputAction* EatItemAction2;
	UPROPERTY(EditAnywhere , BlueprintReadOnly , Category = "Input")
	class UInputAction* UseEquippedItemAction2;

	float LastUpTime2{0.f};
	float LastDownTime2{0.f};
	float LastRightTime2{0.f};
	float LastLeftTime2{0.f};
	bool bUpPressed2{false};
	bool bDownPressed2{false};
	bool bRightPressed2{false};
	bool bLeftPressed2{false};


	///////////////////////////////////
	void CheckEachCollision();
};