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
	
	UPROPERTY(BlueprintReadWrite)
	float Speed{5.f};
	UPROPERTY(BlueprintReadWrite)
	float CurrentSpeed{5.f};
	UPROPERTY(BlueprintReadWrite)
	float TrappedSpeed{1.f};
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
	int32 BalloonRange{2};
	UPROPERTY(BlueprintReadWrite)
	int32 BalloonCount{2};
	
	UPROPERTY()
	FTimerHandle TrappedTimerHandle;
	UPROPERTY()
	FTimerHandle ShieldTimerHandle;
	UPROPERTY()
	FTimerHandle GodTimerHandle;
	UPROPERTY()
	FTimerHandle StopTimerHandle;
	
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
