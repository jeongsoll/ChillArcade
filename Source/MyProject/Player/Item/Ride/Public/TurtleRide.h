// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseRide.h"
#include "TurtleRide.generated.h"

UCLASS()
class MYPROJECT_API ATurtleRide : public ABaseRide {
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATurtleRide();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable)
	void ChangeFast();
	UPROPERTY(EditAnywhere , BlueprintReadWrite)
	class UBoxComponent* Root;
	UPROPERTY(EditAnywhere , BlueprintReadWrite)
	class UStaticMeshComponent* Mesh;
	UPROPERTY(EditAnywhere , BlueprintReadWrite)
	class UStaticMeshComponent* FastMesh;
	
};
