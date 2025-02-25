// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"

#include "AppleAnimation.h"
#include "ArrLocation.h"
#include "BaseRide.h"
#include "BubbleItem.h"
#include "CanItem.h"
#include "DevilItem.h"
#include "FluidItem.h"
#include "LogUtils.h"
#include "MapGen.h"
#include "MyGameInstance.h"
#include "Needle.h"
#include "RangeItem.h"
#include "RollerItem.h"
#include "SendArrInfoManagerComponent.h"
#include "ShieldItem.h"
#include "SpaceShipItem.h"
#include "SpaceShipRide.h"
#include "SpannerItem.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TrappedBalloon.h"
#include "SpawnableShield.h"
#include "TurtleItem.h"
#include "TurtleRide.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BaseMeshObject
		(TEXT("/Game/Player/Model/Apple/ske_apple.ske_apple"));
	if (BaseMeshObject.Succeeded()) {
		GetMesh()->SetSkeletalMesh(BaseMeshObject.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0.f , 0.f , -90.f) , FRotator(0.f , -90.f , 0.f));
	}

	RidingComponent = CreateDefaultSubobject<UChildActorComponent>(TEXT("Riding"));
	RidingComponent->SetupAttachment(GetMesh());
	RidingComponent->SetRelativeLocation(FVector(0.f , 0.f , -45.f));

	TrappedComponent = CreateDefaultSubobject<UChildActorComponent>(TEXT("TrappedComponent"));
	TrappedComponent->SetupAttachment(GetMesh());

	ShieldComponent = CreateDefaultSubobject<UChildActorComponent>(TEXT("ShieldComponent"));
	ShieldComponent->SetupAttachment(GetMesh());

	ConstructorHelpers::FClassFinder<ATrappedBalloon> TempTrappedBalloon
		(TEXT("/Game/Player/Balloon/BP_TrappedBalloon.BP_TrappedBalloon_C"));
	if (TempTrappedBalloon.Succeeded()) {
		TrapBalloonClass = TempTrappedBalloon.Class;
	}

	ConstructorHelpers::FClassFinder<ASpawnableShield> TempShield
		(TEXT("/Game/Player/Item/BP_SpawnableShield.BP_SpawnableShield_C"));
	if (TempShield.Succeeded()) {
		ShieldClass = TempShield.Class;
	}

	// static ConstructorHelpers::FClassFinder<UAppleAnimation> AnimBP
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

	GetMesh()->CastShadow = false;
	
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	InitPlayer();
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bRotating) {
		RotateYaw += DeltaTime * 5000.f;
		GetMesh()->SetRelativeRotation(FRotator(0.f , RotateYaw , 0.f));
	}
}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABaseCharacter::InitPlayer()
{
	UMyGameInstance* GI{Cast<UMyGameInstance>(GetGameInstance())};
	if (GI && GI->SelectedCharacter && GI->SelectedAnimInstance) {
		GetMesh()->SetSkeletalMesh(GI->SelectedCharacter);
		GetMesh()->SetAnimInstanceClass(GI->SelectedAnimInstance);
	}
	
	Speed = PLAYER_INITIAL_SPEED;
	CurrentSpeed = Speed;
}

void ABaseCharacter::SetBalloon()
{
	FArrLocation BalloonLoc;
	BalloonLoc.X = FMath::FloorToInt(MAP_ROW_MAX - GetActorLocation().X / 100);
	BalloonLoc.Y = FMath::FloorToInt(GetActorLocation().Y / 100);

	// 설지할 수 있는지 확인
	int32 MapInfo{SendArrComponent->Map->GameMap[BalloonLoc.X][BalloonLoc.Y]};
	if (BalloonCount > 0 && MapInfo % 100 != EMapType::BalloonLoc
		&& MapInfo != EMapType::Destroyable
		&& MapInfo != EMapType::Pushable) {
		--BalloonCount;

		if (SendArrComponent) {
			SendArrComponent->SendBalloonLocation(BalloonLoc);
		}
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
	LogUtils::Log();

	if (bHasNeedle && bIsTrapped) {
		Escaped();
		bHasNeedle = false;
	}
	if (bHasShield && !bIsTrapped) {
		SetShield();
		bHasShield = false;
	}
	if (bHasCan && !bIsTrapped) {
		ATurtleRide* Turtle{Cast<ATurtleRide>(RidingComponent->GetChildActor())};
		if (CheckRide() && Turtle) {
			Turtle->ChangeFast();
			bHasCan = false;
			Rotating();
		}
	}
	if (bHasSpanner && !bIsTrapped) {
		ASpaceShipRide* SpaceShip{Cast<ASpaceShipRide>(RidingComponent->GetChildActor())};
		if (CheckRide() && SpaceShip) {
			SpaceShip->ChangeFast();
			bHasSpanner = false;
			Rotating();
		}
	}
}

void ABaseCharacter::UseEquipItem(int32 Input)
{
	ATurtleRide* Turtle{Cast<ATurtleRide>(RidingComponent->GetChildActor())};
	ASpaceShipRide* SpaceShip{Cast<ASpaceShipRide>(RidingComponent->GetChildActor())};

	switch (Input) {
	case EItemType::Can:
		if (CheckRide() && Turtle && !bIsTrapped) {
			Turtle->ChangeFast();
			Rotating();
		}
		break;
	case EItemType::Needle:
		if (bIsTrapped) {
			Escaped();
		}
		break;
	case EItemType::Shield:
		if (!bIsTrapped) {
			SetShield();
		}
		break;
	case EItemType::Spanner:
		if (CheckRide() && SpaceShip && !bIsTrapped) {
			SpaceShip->ChangeFast();
			Rotating();
		}
		break;
	case EItemType::Five:
		LogUtils::Log("Input 5 !");
		break;
	case EItemType::Six:
		LogUtils::Log("Input 6 !");
		break;
	default:
		LogUtils::Log("Input Error ! ! ! ! !");
		break;
	}
}

void ABaseCharacter::SetRide(TSubclassOf<class ABaseRide> Ride)
{
	EquippedRideClass = Ride;

	CurrentSpeed = SLOW_RIDE_SPEED;

	// 캐릭터 올리기
	GetMesh()->AddLocalOffset(FVector(0 , 0 , 90.f));
	RidingComponent->SetChildActorClass(Ride);

	StopMovement();
	auto Anim{Cast<UAppleAnimation>(GetMesh()->GetAnimInstance())};
	if (Anim) {
		Anim->OnUp();
	}
}

bool ABaseCharacter::CheckRide()
{
	return !!EquippedRideClass;
}

void ABaseCharacter::RemoveRide()
{
	EquippedRideClass = nullptr;

	CurrentSpeed = Speed;

	// 내리기
	GetMesh()->AddLocalOffset(FVector(0 , 0 , -90.f));
	RidingComponent->SetChildActorClass(nullptr);
}

void ABaseCharacter::Trapped()
{
	if (bIsTrapped || bIsShield || bIsGod) {
		return;
	}

	if (CheckRide()) {
		RemoveRide();
		StopMovement();
		GodMode();

		return;
	}

	bIsTrapped = true;
	TrappedComponent->SetChildActorClass(TrapBalloonClass);
	GetWorldTimerManager().SetTimer(TrappedTimerHandle , this , &ABaseCharacter::Die ,
	                                5.f , false);
	CurrentSpeed = TrappedSpeed;
}

void ABaseCharacter::Escaped()
{
	GodMode();

	bIsTrapped = false;
	TrappedComponent->SetChildActorClass(nullptr);

	GetWorldTimerManager().ClearTimer(TrappedTimerHandle);

	CurrentSpeed = Speed;
}

void ABaseCharacter::Die()
{
	auto Anim{Cast<UAppleAnimation>(GetMesh()->GetAnimInstance())};
	if (Anim) {
		Anim->OnDie();
	}

	// 죽음
	TrappedComponent->SetChildActorClass(nullptr);
	//LogUtils::Log("Die!!!!!");
	GetWorldTimerManager().ClearTimer(TrappedTimerHandle);
}

void ABaseCharacter::SetShield()
{
	bIsShield = true;
	ShieldComponent->SetChildActorClass(ShieldClass);

	GetWorldTimerManager().SetTimer(ShieldTimerHandle , this , &ABaseCharacter::RemoveShield ,
	                                2.f , false);
}

void ABaseCharacter::RemoveShield()
{
	bIsShield = false;
	ShieldComponent->SetChildActorClass(nullptr);

	GetWorldTimerManager().ClearTimer(ShieldTimerHandle);
}

bool ABaseCharacter::HasItem()
{
	return bHasNeedle || bHasShield || bHasCan || bHasSpanner;
}

void ABaseCharacter::GetItem(ABaseItem* BaseItem)
{
	auto Anim{Cast<UAppleAnimation>(GetMesh()->GetAnimInstance())};
	if (Anim) {
		Anim->OnEatItem();
	}

	if (BaseItem->IsA<ABubbleItem>()) {
		if (BalloonCount < MAX_BALLOON_COUNT) {
			++BalloonCount;
		}
	}
	if (BaseItem->IsA<ACanItem>()) {
		ChangeItem(BaseItem);
	}
	if (BaseItem->IsA<ADevilItem>()) {
		Speed = PLAYER_MAX_SPEED;
		if (!bIsTrapped && !CheckRide()) {
			CurrentSpeed = Speed;
		}
	}
	if (BaseItem->IsA<AFluidItem>()) {
		if (BalloonRange <= MAX_BALLOON_RANGE) {
			++BalloonRange;
		}
	}
	if (BaseItem->IsA<ANeedle>()) {
		ChangeItem(BaseItem);
	}
	if (BaseItem->IsA<ARangeItem>()) {
		BalloonRange = MAX_BALLOON_RANGE;
	}
	if (BaseItem->IsA<ARollerItem>()) {
		if (Speed <= PLAYER_MAX_SPEED) {
			++Speed;
		}
		if (!bIsTrapped && !CheckRide()) {
			CurrentSpeed = Speed;
		}
	}
	if (BaseItem->IsA<AShieldItem>()) {
		ChangeItem(BaseItem);
	}
	if (BaseItem->IsA<ASpaceShipItem>()) {
		if (!CheckRide()) {
			auto SpaceShip{Cast<ASpaceShipItem>(BaseItem)};
			SpaceShip->EquipPlayerSpaceShip(this);
		}
	}
	if (BaseItem->IsA<ASpannerItem>()) {
		ChangeItem(BaseItem);
	}
	if (BaseItem->IsA<ATurtleItem>()) {
		if (!CheckRide()) {
			auto Turtle{Cast<ATurtleItem>(BaseItem)};
			Turtle->EquipPlayerTurtle(this);
		}
	}
}


void ABaseCharacter::RecoverBalloon()
{
	++BalloonCount;
}

void ABaseCharacter::ChangeItem(ABaseItem* BaseItem)
{
	if (BaseItem->IsA<ACanItem>()) {
		bHasCan = true;
		bHasNeedle = false;
		bHasShield = false;
		bHasSpanner = false;
	}
	if (BaseItem->IsA<ANeedle>()) {
		bHasCan = false;
		bHasNeedle = true;
		bHasShield = false;
		bHasSpanner = false;
	}
	if (BaseItem->IsA<AShieldItem>()) {
		bHasCan = false;
		bHasNeedle = false;
		bHasShield = true;
		bHasSpanner = false;
	}
	if (BaseItem->IsA<ASpannerItem>()) {
		bHasCan = false;
		bHasNeedle = false;
		bHasShield = false;
		bHasSpanner = true;
	}
}

void ABaseCharacter::DisableGodMode()
{
	bIsGod = false;
	GetWorldTimerManager().ClearTimer(GodTimerHandle);
}

void ABaseCharacter::GodMode()
{
	bIsGod = true;

	// 캐릭터 반짝반짝

	GetWorldTimerManager().SetTimer(GodTimerHandle , this , &ABaseCharacter::DisableGodMode ,
	                                0.55f , false);
}

void ABaseCharacter::StopMovement()
{
	CurrentSpeed = 0.f;

	GetWorldTimerManager().SetTimer(StopTimerHandle , this , &ABaseCharacter::StartMovement ,
	                                0.8f , false);
}

void ABaseCharacter::StartMovement()
{
	if (CheckRide()) {
		CurrentSpeed = SLOW_RIDE_SPEED;
	}
	else {
		CurrentSpeed = Speed;
	}
}

void ABaseCharacter::UpgradeRide()
{
	CurrentSpeed = FAST_RIDE_SPEED;
	bRotating = false;
	GetMesh()->SetRelativeRotation(FRotator(0.f , 90 , 0.f));
	GetWorldTimerManager().ClearTimer(UpgradeTimerHandle);
}

bool ABaseCharacter::CheckIfSpaceShip()
{
	if (CheckRide()) {
		if (RidingComponent->GetChildActor()->IsA<ASpaceShipRide>()) {
			return true;
		}
	}

	return false;
}

void ABaseCharacter::Rotating()
{
	CurrentSpeed = 0.f;
	bRotating = true;
	GetWorldTimerManager().SetTimer(UpgradeTimerHandle , this , &ABaseCharacter::UpgradeRide ,
	                                1.f , false);
}
