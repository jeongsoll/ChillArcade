// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
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
	void InitPlayer();
	UFUNCTION(BlueprintCallable)
	void SetBalloon();
	UFUNCTION(BlueprintCallable)
	struct FArrLocation CheckLocation();
	UFUNCTION(BlueprintCallable)
	void UseEatItem();
	UFUNCTION(BlueprintCallable)
	void UseEquipItem(int32 Input);
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
	UFUNCTION(BlueprintCallable)
	bool HasItem();
	UFUNCTION(BlueprintCallable)
	void GetItem(ABaseItem* BaseItem);
	UFUNCTION(BlueprintCallable)
	void RecoverBalloon();
	UFUNCTION(BlueprintCallable)
	void ChangeItem(ABaseItem* BaseItem);
	UFUNCTION(BlueprintCallable)
	void DisableGodMode();
	UFUNCTION(BlueprintCallable)
	void GodMode();
	UFUNCTION(BlueprintCallable)
	void StopMovement();
	UFUNCTION(BlueprintCallable)
	void StartMovement();
	UFUNCTION(BlueprintCallable)
	void UpgradeRide();
	UFUNCTION(BlueprintCallable)
	bool CheckIfSpaceShip();
	UFUNCTION(BlueprintCallable)
	void Rotating();
	
	UPROPERTY(BlueprintReadWrite)
	float Speed{3.f};
	UPROPERTY(BlueprintReadWrite)
	float CurrentSpeed{3.f};
	UPROPERTY(BlueprintReadWrite)
	float TrappedSpeed{0.6f};
	UPROPERTY(BlueprintReadWrite)
	bool bHasRiding{false};
	UPROPERTY(BlueprintReadWrite)
	bool bIsTrapped{false};
	UPROPERTY(BlueprintReadWrite)
	bool bIsShield{false};
	UPROPERTY(BlueprintReadWrite)
	bool bHasNeedle{false};
	UPROPERTY(BlueprintReadWrite)
	bool bHasShield{false};
	UPROPERTY(BlueprintReadWrite)
	bool bHasCan{false};
	UPROPERTY(BlueprintReadWrite)
	bool bHasSpanner{false};
	UPROPERTY(BlueprintReadWrite)
	bool bIsGod{false};
	UPROPERTY(BlueprintReadWrite)
	float RotateTime{};
	UPROPERTY(BlueprintReadWrite)
	float RotateEndTime{0.5f};
	UPROPERTY(BlueprintReadWrite)
	bool bRotating{false};
	UPROPERTY(BlueprintReadWrite)
	float RotateYaw{0.f};
	
	UPROPERTY(BlueprintReadWrite)
	int32 BalloonRange{1};
	UPROPERTY(BlueprintReadWrite)
	int32 BalloonCount{1};
	
	UPROPERTY()
	FTimerHandle TrappedTimerHandle;
	UPROPERTY()
	FTimerHandle ShieldTimerHandle;
	UPROPERTY()
	FTimerHandle GodTimerHandle;
	UPROPERTY()
	FTimerHandle StopTimerHandle;
	UPROPERTY()
	FTimerHandle UpgradeTimerHandle;
	
	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<ABaseRide> EquippedRideClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UChildActorComponent* RidingComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UChildActorComponent* TrappedComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UChildActorComponent* ShieldComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ATrappedBalloon> TrapBalloonClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ASpawnableShield> ShieldClass;
};
