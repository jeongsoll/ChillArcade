// Fill out your copyright notice in the Description page of Project Settings.


#include "WinWidget.h"

#include "Animation/WidgetAnimation.h"
#include "Kismet/GameplayStatics.h"

void UWinWidget::PlayWinAnimation()
{
	UE_LOG(LogTemp, Warning, TEXT("Playing WinAnimation"));
	PlayAnimation(WinAnimation);
	GetWorld()->GetTimerManager().SetTimer(OpenLevelTimer, this, &UWinWidget::OpenWaitRoomLevel, 3, false);
}

void UWinWidget::OpenWaitRoomLevel()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("WaitRoomLevel"));
}
