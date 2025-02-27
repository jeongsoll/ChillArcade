// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WinWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UWinWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* WinAnimation;
	
	UFUNCTION(BlueprintCallable)
	void PlayWinAnimation();
};
