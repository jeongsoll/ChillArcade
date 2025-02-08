// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "LogUtils.h"
#include "SendArrInfoManagerComponent.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BaseMeshObject
		(TEXT("/Game/Player/Model/FBX_format/character-male-b.character-male-b"));
	if (BaseMeshObject.Succeeded()) {
		GetMesh()->SetSkeletalMesh(BaseMeshObject.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0.f , 0.f , -90.f) , FRotator(0.f , -90.f , 0.f));
	}

	SendArrComponent = CreateDefaultSubobject<USendArrInfoManagerComponent>(
		TEXT("SendArrManager")
	);
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABaseCharacter::SetBalloon()
{
	const int X = FMath::FloorToInt(GetActorLocation().X / 100);
	const int Y = FMath::FloorToInt(GetActorLocation().Y / 100);

	if (SendArrComponent) {
		SendArrComponent->SendBalloonLocation(X , Y);
	}
}

void ABaseCharacter::CheckLocation()
{
	const int X = FMath::FloorToInt(GetActorLocation().X / 100);
	const int Y = FMath::FloorToInt(GetActorLocation().Y / 100);

	if (SendArrComponent) {
		SendArrComponent->SendPlayerLocation(X , Y);
	}
}

void ABaseCharacter::UseEatItem()
{
	//
}

void ABaseCharacter::UseEquipItem()
{
	//
}
