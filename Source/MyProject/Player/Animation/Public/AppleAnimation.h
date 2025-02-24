// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AppleAnimation.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UAppleAnimation : public UAnimInstance {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, Category = Anim)
	void OnEatItem();
	UFUNCTION(BlueprintImplementableEvent, Category = Anim)
	void OnTouch();
	UFUNCTION(BlueprintImplementableEvent, Category = Anim)
	void OnDie();
	UFUNCTION(BlueprintImplementableEvent, Category = Anim)
	void OnUp();
};
