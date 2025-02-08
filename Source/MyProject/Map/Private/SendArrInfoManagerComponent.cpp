// Fill out your copyright notice in the Description page of Project Settings.


#include "SendArrInfoManagerComponent.h"

#include "LogUtils.h"


// Sets default values for this component's properties
USendArrInfoManagerComponent::USendArrInfoManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USendArrInfoManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USendArrInfoManagerComponent::TickComponent(float DeltaTime , ELevelTick TickType ,
                                                 FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime , TickType , ThisTickFunction);

	// ...
}

void USendArrInfoManagerComponent::SendPlayerLocation(int X , int Y)
{
	LogUtils::Log("Location Array : ", X, Y);
}

void USendArrInfoManagerComponent::SendBalloonLocation(int X, int Y)
{
	LogUtils::Log("Balloon Array : ", X, Y);
}

