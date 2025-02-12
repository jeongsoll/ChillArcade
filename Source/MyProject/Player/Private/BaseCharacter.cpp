// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "ArrLocation.h"
#include "BaseRide.h"
#include "BaseWaterBalloon.h"
#include "LogUtils.h"
#include "SendArrInfoManagerComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Slate/SGameLayerManager.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BaseMeshObject
		(TEXT("/Game/Player/Model/FBX_format/character-male-f.character-male-f"));
	if (BaseMeshObject.Succeeded()) {
		GetMesh()->SetSkeletalMesh(BaseMeshObject.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0.f , 0.f , -90.f) , FRotator(0.f , -90.f , 0.f));
	}

	RidingComponent = CreateDefaultSubobject<UChildActorComponent>(TEXT("Riding"));
	RidingComponent->SetupAttachment(GetMesh());
	RidingComponent->SetRelativeLocation(FVector(0.f , 0.f , -45.f));

	// static ConstructorHelpers::FClassFinder<UAnimInstance> AnimBP
	// 	(TEXT("/Game/Player/Animation/ABP_AppleAnimation.ABP_AppleAnimation_C"));
	// if (AnimBP.Succeeded()) {
	// 	GetMesh()->SetAnimInstanceClass(AnimBP.Class);
	// }

	SendArrComponent = CreateDefaultSubobject<USendArrInfoManagerComponent>(
		TEXT("SendArrManager")
	);

	// 캐릭터 이동에 따라 회전
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate.Yaw = 1440.f;
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

	//Speed = GetVelocity().Length();
}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABaseCharacter::SetBalloon()
{
	// 설지할 수 있는지 확인

	FArrLocation BalloonLoc;
	BalloonLoc.X = FMath::FloorToInt(MAP_ROW_MAX - GetActorLocation().X / 100);
	BalloonLoc.Y = FMath::FloorToInt(GetActorLocation().Y / 100);

	if (SendArrComponent) {
		SendArrComponent->SendBalloonLocation(BalloonLoc);
	}
}

// 캐릭터 위치 확인
struct FArrLocation ABaseCharacter::CheckLocation()
{
	FArrLocation PlayerLoc;
	PlayerLoc.X = FMath::FloorToInt(MAP_ROW_MAX - GetActorLocation().X / 100);
	PlayerLoc.Y = FMath::FloorToInt(GetActorLocation().Y / 100);

	if (SendArrComponent) {
		SendArrComponent->SendPlayerLocation(PlayerLoc);
	}

	return PlayerLoc;
}

void ABaseCharacter::UseEatItem()
{
	//
}

void ABaseCharacter::UseEquipItem()
{
	//
}

void ABaseCharacter::SetRide(TSubclassOf<class ABaseRide> Ride)
{
	EquippedRideClass = Ride;

	// 올리기
	GetMesh()->AddLocalOffset(FVector(0 , 0 , 90.f));
	RidingComponent->SetChildActorClass(Ride);
}

bool ABaseCharacter::CheckRide()
{
	return !!EquippedRideClass;
}

void ABaseCharacter::RemoveRide()
{
	EquippedRideClass = nullptr;

	// 내리기
	GetMesh()->AddLocalOffset(FVector(0 , 0 , -90.f));
	RidingComponent->SetChildActorClass(nullptr);
}
