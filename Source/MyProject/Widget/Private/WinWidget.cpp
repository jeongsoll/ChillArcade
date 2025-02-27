// Fill out your copyright notice in the Description page of Project Settings.


#include "WinWidget.h"

#include "Animation/WidgetAnimation.h"

void UWinWidget::PlayWinAnimation()
{
	UE_LOG(LogTemp, Warning, TEXT("Playing WinAnimation"));
	PlayAnimation(WinAnimation);
}
