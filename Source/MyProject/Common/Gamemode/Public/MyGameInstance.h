// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UMyGameInstance : public UGameInstance {
	GENERATED_BODY()

	UMyGameInstance();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USkeletalMesh* SelectedCharacter;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UMaterial* SelectedCharacterMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor SelectedColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UClass* SelectedAnimInstance;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMesh* SelectedBalloon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMesh* SelectedBalloonRange;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> MadeMap;
};
