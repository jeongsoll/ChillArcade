// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseWall.h"
#include "GameFramework/Actor.h"
#include "Bush.generated.h"

UCLASS()
class MYPROJECT_API ABush : public ABaseWall
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABush();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	bool bBushDestroyed = false;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual bool IsBroken() const override { return bBroken || bBushDestroyed;};
	virtual void SetBroken(bool bNewState) override { bBushDestroyed = bNewState;};
	
};
