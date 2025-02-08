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
	virtual void SetupInputComponent() override;
	void MovePlayer(const struct FInputActionValue &Value);
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* MoveAction;
};
