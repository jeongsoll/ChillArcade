// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

UCLASS()
class MYPROJECT_API ABaseCharacter : public ACharacter {
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	class USendArrInfoManagerComponent* SendArrComponent;
	
	UFUNCTION(BlueprintCallable)
	void SetBalloon();
	UFUNCTION(BlueprintCallable)
	struct FArrLocation CheckLocation();
	UFUNCTION(BlueprintCallable)
	void UseEatItem();
	UFUNCTION(BlueprintCallable)
	void UseEquipItem();
	UFUNCTION(BlueprintCallable)
	void SetRide(TSubclassOf<class ABaseRide> Ride);
	UFUNCTION(BlueprintCallable)
	bool CheckRide();
	UFUNCTION(BlueprintCallable)
	void RemoveRide();
	UFUNCTION(BlueprintCallable)
	void Trapped();
	UFUNCTION(BlueprintCallable)
	void Escaped();
	UFUNCTION(BlueprintCallable)
	void Die();
	UFUNCTION(BlueprintCallable)
	void SetShield();
	UFUNCTION(BlueprintCallable)
	void RemoveShield();
	
	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<ABaseRide> EquippedRideClass;
	UPROPERTY(BlueprintReadWrite)
	float Speed;
	UPROPERTY(BlueprintReadWrite)
	bool bHasRiding{false};
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UChildActorComponent* RidingComponent;
	UPROPERTY(BlueprintReadWrite)
	bool bIsTrapped{false};
	UPROPERTY()
	FTimerHandle TrappedTimerHandle;
	UPROPERTY()
	FTimerHandle ShieldTimerHandle;
	UPROPERTY(BlueprintReadWrite)
	bool bIsShield{false};
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ATrappedBalloon> TrapBalloonClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UChildActorComponent* TrappedComponent;
};
