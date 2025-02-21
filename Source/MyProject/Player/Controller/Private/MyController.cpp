// Fill out your copyright notice in the Description page of Project Settings.


#include "MyController.h"

#include "ArrLocation.h"
#include "BaseCharacter.h"
#include "InputMappingContext.h" /*UInputMappingContext*/
#include "EnhancedInputSubsystems.h" /*UEnhancedInputLocalPlayerSubsystem*/
#include "EnhancedInputComponent.h" /*UEnhancedInputComponent*/
#include "MapGen.h"
#include "LogUtils.h"
#include "SpawnItem.h"
#include "Kismet/GameplayStatics.h"

AMyController::AMyController()
{
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> ImcPlayer
		(TEXT("/Game/Player/Input/IMC_Player.IMC_Player"));
	if (ImcPlayer.Succeeded()) {
		InputMappingContext = ImcPlayer.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionMoveUp
		(TEXT("/Game/Player/Input/IA_MoveUp.IA_MoveUp"));
	if (InputActionMoveUp.Succeeded()) {
		MoveUp = InputActionMoveUp.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionMoveDown
		(TEXT("/Game/Player/Input/IA_MoveDown.IA_MoveDown"));
	if (InputActionMoveDown.Succeeded()) {
		MoveDown = InputActionMoveDown.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionMoveRight
		(TEXT("/Game/Player/Input/IA_MoveRight.IA_MoveRight"));
	if (InputActionMoveRight.Succeeded()) {
		MoveRight = InputActionMoveRight.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionMoveLeft
		(TEXT("/Game/Player/Input/IA_MoveLeft.IA_MoveLeft"));
	if (InputActionMoveLeft.Succeeded()) {
		MoveLeft = InputActionMoveLeft.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionBalloon
		(TEXT("/Game/Player/Input/IA_Balloon.IA_Balloon"));
	if (InputActionBalloon.Succeeded()) {
		BalloonAction = InputActionBalloon.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionEatItem
		(TEXT("/Game/Player/Input/IA_EatItem.IA_EatItem"));
	if (InputActionEatItem.Succeeded()) {
		EatItemAction = InputActionEatItem.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionEquippedItem
		(TEXT("/Game/Player/Input/IA_Item.IA_Item"));
	if (InputActionEquippedItem.Succeeded()) {
		UseEquippedItemAction = InputActionEquippedItem.Object;
	}

	// static ConstructorHelpers::FClassFinder<AMapGen> MapClass
	// 	(TEXT("/Game/Map/Blueprints/BP_Map.BP_Map_C"));
	// if (MapClass.Succeeded()) {
	// 	MapGenClass = MapClass.Class;
	// }
}

void AMyController::BeginPlay()
{
	Super::BeginPlay();

	MapGen = Cast<AMapGen>(UGameplayStatics::GetActorOfClass(GetWorld() , AMapGen::StaticClass()));

	if (UEnhancedInputLocalPlayerSubsystem* subsystem{
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer())
	}) {
		subsystem->AddMappingContext(InputMappingContext , 0);
	}

	ControlledPlayer = Cast<ABaseCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
}

void AMyController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ControlledPlayer && !Direction.IsZero()) {
		float RotateValue{};
		if (1 == Direction.X) {
			RotateValue = -90;
		}
		if (-1 == Direction.X) {
			RotateValue = 90;
		}
		if (1 == Direction.Y) {
			RotateValue = 0;
		}
		if (-1 == Direction.Y) {
			RotateValue = 180;
		}
		ControlledPlayer->GetMesh()->SetRelativeRotation(FRotator(0 , RotateValue , 0));

		if (CheckCollision()) {
			ControlledPlayer->AddActorLocalOffset(Direction * ControlledPlayer->CurrentSpeed);
			ControlledPlayer->CheckLocation();
		}

		CheckItem();
	}
}

void AMyController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent{CastChecked<UEnhancedInputComponent>(InputComponent)}) {
		EnhancedInputComponent->BindAction(BalloonAction , ETriggerEvent::Started , this , &AMyController::SetBalloon);
		EnhancedInputComponent->BindAction(EatItemAction , ETriggerEvent::Started , this , &AMyController::UseEatItem);

		EnhancedInputComponent->BindAction(MoveUp , ETriggerEvent::Started , this , &AMyController::MovePlayerUp);
		EnhancedInputComponent->BindAction(MoveDown , ETriggerEvent::Started , this , &AMyController::MovePlayerDown);
		EnhancedInputComponent->BindAction(MoveRight , ETriggerEvent::Started , this , &AMyController::MovePlayerRight);
		EnhancedInputComponent->BindAction(MoveLeft , ETriggerEvent::Started , this , &AMyController::MovePlayerLeft);

		EnhancedInputComponent->BindAction(MoveUp , ETriggerEvent::Completed , this ,
		                                   &AMyController::MoveUpCompleted);
		EnhancedInputComponent->BindAction(MoveDown , ETriggerEvent::Completed , this ,
		                                   &AMyController::MoveDownCompleted);
		EnhancedInputComponent->BindAction(MoveRight , ETriggerEvent::Completed , this ,
		                                   &AMyController::MoveRightCompleted);
		EnhancedInputComponent->BindAction(MoveLeft , ETriggerEvent::Completed , this ,
		                                   &AMyController::MoveLeftCompleted);

		EnhancedInputComponent->BindAction(UseEquippedItemAction , ETriggerEvent::Triggered , this ,
		                                   &AMyController::UseEquippedItem);
	}
}

void AMyController::SetBalloon(const struct FInputActionValue& Value)
{
	if (ControlledPlayer) {
		ControlledPlayer->SetBalloon();
	}
}

void AMyController::UseEatItem(const struct FInputActionValue& Value)
{
	if (ControlledPlayer) {
		ControlledPlayer->UseEatItem();
	}
}

void AMyController::MovePlayerUp(const struct FInputActionValue& Value)
{
	LastUpTime = GetWorld()->GetTimeSeconds();
	bUpPressed = true;
	UpdateDirection();
}

void AMyController::MovePlayerDown(const struct FInputActionValue& Value)
{
	LastDownTime = GetWorld()->GetTimeSeconds();
	bDownPressed = true;
	UpdateDirection();
}

void AMyController::MovePlayerRight(const struct FInputActionValue& Value)
{
	LastRightTime = GetWorld()->GetTimeSeconds();
	bRightPressed = true;
	UpdateDirection();
}

void AMyController::MovePlayerLeft(const struct FInputActionValue& Value)
{
	LastLeftTime = GetWorld()->GetTimeSeconds();
	bLeftPressed = true;
	UpdateDirection();
}

void AMyController::MoveUpCompleted(const FInputActionValue& Value)
{
	bUpPressed = false;
	UpdateDirection();
}

void AMyController::MoveDownCompleted(const FInputActionValue& Value)
{
	bDownPressed = false;
	UpdateDirection();
}

void AMyController::MoveRightCompleted(const FInputActionValue& Value)
{
	bRightPressed = false;
	UpdateDirection();
}

void AMyController::MoveLeftCompleted(const FInputActionValue& Value)
{
	bLeftPressed = false;
	UpdateDirection();
}

void AMyController::UseEquippedItem(const FInputActionValue& Value)
{
	float Input{Value.Get<float>()};
	ControlledPlayer->UseEquipItem(FMath::FloorToInt32(Input));
}

void AMyController::UpdateDirection()
{
	TMap<float , FVector> LatestInputs;

	if (bUpPressed) {
		LatestInputs.Add(LastUpTime , FVector(1 , 0 , 0));
	}
	if (bDownPressed) {
		LatestInputs.Add(LastDownTime , FVector(-1 , 0 , 0));
	}
	if (bRightPressed) {
		LatestInputs.Add(LastRightTime , FVector(0 , 1 , 0));
	}
	if (bLeftPressed) {
		LatestInputs.Add(LastLeftTime , FVector(0 , -1 , 0));
	}

	if (LatestInputs.Num() > 0) {
		TArray<float> Times;
		LatestInputs.GetKeys(Times);
		float MaxTime = FMath::Max(Times);
		Direction = LatestInputs[MaxTime];
	}
	else {
		Direction = FVector::ZeroVector;
	}
}

bool AMyController::CheckCollision()
{
	FArrLocation PlayerArray{ControlledPlayer->CheckLocation().X , ControlledPlayer->CheckLocation().Y};
	FVector PlayerLocation{ControlledPlayer->GetActorLocation()};
	float PlayerLength{PLAYER_SIZE};
	//LogUtils::Log("Player Location" , PlayerLocation.X , PlayerLocation.Y);

	FArrLocation CollisionCheckWallArray{
		PlayerArray.X - FMath::FloorToInt(Direction.X) , PlayerArray.Y + FMath::FloorToInt(Direction.Y)
	};
	FVector WallLocation{
		MapGen->ArrayToWorldLocation(CollisionCheckWallArray)
	};

	uint8 WallType{static_cast<uint8>(MapGen->GameMap[CollisionCheckWallArray.X][CollisionCheckWallArray.Y])};
	if (ControlledPlayer->CheckIfSpaceShip()) {
		if (WallType == EMapType::Blocking || WallType == EMapType::BalloonLoc) {
			if (FMath::Abs(Direction.X) == 1 && FMath::Abs(WallLocation.X - PlayerLocation.X) < PlayerLength) {
				return false;
			}
			if (FMath::Abs(Direction.Y) == 1 && FMath::Abs(WallLocation.Y - PlayerLocation.Y) < PlayerLength) {
				return false;
			}
		}
	}
	else {
		if (WallType == EMapType::Blocking || WallType == EMapType::Destroyable || WallType == EMapType::BalloonLoc) {
			if (FMath::Abs(Direction.X) == 1 && FMath::Abs(WallLocation.X - PlayerLocation.X) < PlayerLength) {
				return false;
			}
			if (FMath::Abs(Direction.Y) == 1 && FMath::Abs(WallLocation.Y - PlayerLocation.Y) < PlayerLength) {
				return false;
			}
		}
	}

	return true;
}

void AMyController::CheckItem()
{
	if (ControlledPlayer->CheckIfSpaceShip()) {
		return;
	}
	
	FArrLocation PlayerArray{ControlledPlayer->CheckLocation().X , ControlledPlayer->CheckLocation().Y};
	if (EMapType::BubbleItem <= MapGen->GameMap[PlayerArray.X][PlayerArray.Y] && MapGen->GameMap[PlayerArray.X][
		PlayerArray.Y] <= EMapType::TurtleItem) {
		ControlledPlayer->GetItem(MapGen->ItemMap[PlayerArray.X][PlayerArray.Y]);

		MapGen->UpdateMapDestroyed(PlayerArray);
	}
}
