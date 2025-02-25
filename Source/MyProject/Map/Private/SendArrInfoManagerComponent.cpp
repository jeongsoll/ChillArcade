// Fill out your copyright notice in the Description page of Project Settings.


#include "SendArrInfoManagerComponent.h"
#include "ArrLocation.h"
#include "LogUtils.h"
#include "MapGen.h"
#include "Kismet/GameplayStatics.h"


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

	Map = Cast<AMapGen>(UGameplayStatics::GetActorOfClass(GetWorld(), AMapGen::StaticClass()));
	// ...
}


// Called every frame
void USendArrInfoManagerComponent::TickComponent(float DeltaTime , ELevelTick TickType ,
                                                 FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime , TickType , ThisTickFunction);

	// ...
}

void USendArrInfoManagerComponent::SendPlayerLocation(struct FArrLocation Loc)
{
	//LogUtils::Log("Location Array : " , Loc.X , Loc.Y);
	Map->UpdateMapPlayer(Loc);
}

void USendArrInfoManagerComponent::SendBalloonLocation(struct FArrLocation Loc)
{
	//LogUtils::Log("Balloon Array : " , Loc.X , Loc.Y);
	Map->UpdateMapBalloon(Loc);
}

void USendArrInfoManagerComponent::SendBalloonExplodeLocation(TArray<struct FArrLocation> Loc)
{
	// for (const auto& [X, Y] : Loc) {
	// 	LogUtils::Log("Explode Location : " , X , Y);
	// }
	Map->UpdateMapBalloonStream(Loc);
}

void USendArrInfoManagerComponent::SendRemoveLocation(struct FArrLocation Loc)
{
	//LogUtils::Log("Remove Location : " , Loc.X , Loc.Y);
	Map->UpdateMapDestroyed(Loc);
}

void USendArrInfoManagerComponent::SendBalloonExplodeLocation_AI(TArray<struct FArrLocation> Loc)
{
	
}
