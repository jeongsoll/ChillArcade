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

	//////////////////////////////////////////////////////////////////////////////////////////

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionMoveUp2
		(TEXT("/Game/Player/Input/IA_MoveUp2.IA_MoveUp2"));
	if (InputActionMoveUp2.Succeeded()) {
		MoveUp2 = InputActionMoveUp2.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionMoveDown2
		(TEXT("/Game/Player/Input/IA_MoveDown2.IA_MoveDown2"));
	if (InputActionMoveDown2.Succeeded()) {
		MoveDown2 = InputActionMoveDown2.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionMoveRight2
		(TEXT("/Game/Player/Input/IA_MoveRight2.IA_MoveRight2"));
	if (InputActionMoveRight2.Succeeded()) {
		MoveRight2 = InputActionMoveRight2.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionMoveLeft2
		(TEXT("/Game/Player/Input/IA_MoveLeft2.IA_MoveLeft2"));
	if (InputActionMoveLeft2.Succeeded()) {
		MoveLeft2 = InputActionMoveLeft2.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionBalloon2
		(TEXT("/Game/Player/Input/IA_Balloon2.IA_Balloon2"));
	if (InputActionBalloon2.Succeeded()) {
		BalloonAction2 = InputActionBalloon2.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionEatItem2
		(TEXT("/Game/Player/Input/IA_EatItem2.IA_EatItem2"));
	if (InputActionEatItem2.Succeeded()) {
		EatItemAction2 = InputActionEatItem2.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionEquippedItem2
		(TEXT("/Game/Player/Input/IA_Item2.IA_Item2"));
	if (InputActionEquippedItem2.Succeeded()) {
		UseEquippedItemAction2 = InputActionEquippedItem2.Object;
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
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld() , TEXT("Player2") , FoundActors);
	ControlledPlayer2 = Cast<ABaseCharacter>(FoundActors[0]);
}

void AMyController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckEachCollision();
	
	if (ControlledPlayer && !ControlledPlayer->Direction.IsZero()) {
		float RotateValue{};
		if (1 == ControlledPlayer->Direction.X) {
			RotateValue = -90;
		}
		if (-1 == ControlledPlayer->Direction.X) {
			RotateValue = 90;
		}
		if (1 == ControlledPlayer->Direction.Y) {
			RotateValue = 0;
		}
		if (-1 == ControlledPlayer->Direction.Y) {
			RotateValue = 180;
		}
		ControlledPlayer->GetMesh()->SetRelativeRotation(FRotator(0 , RotateValue , 0));

		if (CheckCollision()) {
			ControlledPlayer->AddActorLocalOffset(ControlledPlayer->Direction * ControlledPlayer->CurrentSpeed);
			//LogUtils::Log("Speed", ControlledPlayer->CurrentSpeed);
			ControlledPlayer->CheckLocation();
		}

		CheckItem();
	}

	if (ControlledPlayer2 && !ControlledPlayer2->Direction.IsZero()) {
		float RotateValue{};
		if (1 == ControlledPlayer2->Direction.X) {
			RotateValue = -90;
		}
		if (-1 == ControlledPlayer2->Direction.X) {
			RotateValue = 90;
		}
		if (1 == ControlledPlayer2->Direction.Y) {
			RotateValue = 0;
		}
		if (-1 == ControlledPlayer2->Direction.Y) {
			RotateValue = 180;
		}
		ControlledPlayer2->GetMesh()->SetRelativeRotation(FRotator(0 , RotateValue , 0));

		if (CheckCollision2()) {
			ControlledPlayer2->AddActorLocalOffset(ControlledPlayer2->Direction * ControlledPlayer2->CurrentSpeed);
			//LogUtils::Log("Speed", ControlledPlayer->CurrentSpeed);
			ControlledPlayer2->CheckLocation();
		}

		CheckItem2();
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

		/////////////////////////////////////////////////////////////////////////////

		EnhancedInputComponent->BindAction(BalloonAction2 , ETriggerEvent::Started , this ,
		                                   &AMyController::SetBalloon2);
		EnhancedInputComponent->BindAction(EatItemAction2 , ETriggerEvent::Started , this ,
		                                   &AMyController::UseEatItem2);

		EnhancedInputComponent->BindAction(MoveUp2 , ETriggerEvent::Started , this , &AMyController::MovePlayerUp2);
		EnhancedInputComponent->BindAction(MoveDown2 , ETriggerEvent::Started , this , &AMyController::MovePlayerDown2);
		EnhancedInputComponent->BindAction(MoveRight2 , ETriggerEvent::Started , this ,
		                                   &AMyController::MovePlayerRight2);
		EnhancedInputComponent->BindAction(MoveLeft2 , ETriggerEvent::Started , this , &AMyController::MovePlayerLeft2);

		EnhancedInputComponent->BindAction(MoveUp2 , ETriggerEvent::Completed , this ,
		                                   &AMyController::MoveUpCompleted2);
		EnhancedInputComponent->BindAction(MoveDown2 , ETriggerEvent::Completed , this ,
		                                   &AMyController::MoveDownCompleted2);
		EnhancedInputComponent->BindAction(MoveRight2 , ETriggerEvent::Completed , this ,
		                                   &AMyController::MoveRightCompleted2);
		EnhancedInputComponent->BindAction(MoveLeft2 , ETriggerEvent::Completed , this ,
		                                   &AMyController::MoveLeftCompleted2);

		EnhancedInputComponent->BindAction(UseEquippedItemAction2 , ETriggerEvent::Triggered , this ,
		                                   &AMyController::UseEquippedItem2);
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
		ControlledPlayer->Direction = LatestInputs[MaxTime];
	}
	else {
		ControlledPlayer->Direction = FVector::ZeroVector;
	}
}

bool AMyController::CheckCollision()
{
	FArrLocation PlayerArray{ControlledPlayer->CheckLocation().X , ControlledPlayer->CheckLocation().Y};
	FVector PlayerLocation{ControlledPlayer->GetActorLocation()};
	float PlayerLength{PLAYER_SIZE};
	//LogUtils::Log("Player Location" , PlayerLocation.X , PlayerLocation.Y);

	FArrLocation CollisionCheckWallArray{
		PlayerArray.X - FMath::FloorToInt(ControlledPlayer->Direction.X) ,
		PlayerArray.Y + FMath::FloorToInt(ControlledPlayer->Direction.Y)
	};
	FVector WallLocation{
		MapGen->ArrayToWorldLocation(CollisionCheckWallArray)
	};

	uint8 WallType{static_cast<uint8>(MapGen->GameMap[CollisionCheckWallArray.X][CollisionCheckWallArray.Y])};
	if (ControlledPlayer->CheckIfSpaceShip()) {
		if (WallType == EMapType::Blocking || WallType == EMapType::BalloonLoc) {
			if (FMath::Abs(ControlledPlayer->Direction.X) == 1 && FMath::Abs(WallLocation.X - PlayerLocation.X) <
				PlayerLength) {
				return false;
			}
			if (FMath::Abs(ControlledPlayer->Direction.Y) == 1 && FMath::Abs(WallLocation.Y - PlayerLocation.Y) <
				PlayerLength) {
				return false;
			}
		}
	}
	else {
		if (WallType == EMapType::Blocking || WallType == EMapType::Destroyable || WallType == EMapType::BalloonLoc) {
			if (FMath::Abs(ControlledPlayer->Direction.X) == 1 && FMath::Abs(WallLocation.X - PlayerLocation.X) <
				PlayerLength) {
				return false;
			}
			if (FMath::Abs(ControlledPlayer->Direction.Y) == 1 && FMath::Abs(WallLocation.Y - PlayerLocation.Y) <
				PlayerLength) {
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


////////////////////////////////////////////////////////


void AMyController::SetBalloon2(const struct FInputActionValue& Value)
{
	if (ControlledPlayer2) {
		ControlledPlayer2->SetBalloon();
	}
}

void AMyController::UseEatItem2(const struct FInputActionValue& Value)
{
	if (ControlledPlayer2) {
		ControlledPlayer2->UseEatItem();
	}
}

void AMyController::MovePlayerUp2(const struct FInputActionValue& Value)
{
	LastUpTime2 = GetWorld()->GetTimeSeconds();
	bUpPressed2 = true;
	UpdateDirection2();
}

void AMyController::MovePlayerDown2(const struct FInputActionValue& Value)
{
	LastDownTime2 = GetWorld()->GetTimeSeconds();
	bDownPressed2 = true;
	UpdateDirection2();
}

void AMyController::MovePlayerRight2(const struct FInputActionValue& Value)
{
	LastRightTime2 = GetWorld()->GetTimeSeconds();
	bRightPressed2 = true;
	UpdateDirection2();
}

void AMyController::MovePlayerLeft2(const struct FInputActionValue& Value)
{
	LastLeftTime2 = GetWorld()->GetTimeSeconds();
	bLeftPressed2 = true;
	UpdateDirection2();
}

void AMyController::MoveUpCompleted2(const FInputActionValue& Value)
{
	bUpPressed2 = false;
	UpdateDirection2();
}

void AMyController::MoveDownCompleted2(const FInputActionValue& Value)
{
	bDownPressed2 = false;
	UpdateDirection2();
}

void AMyController::MoveRightCompleted2(const FInputActionValue& Value)
{
	bRightPressed2 = false;
	UpdateDirection2();
}

void AMyController::MoveLeftCompleted2(const FInputActionValue& Value)
{
	bLeftPressed2 = false;
	UpdateDirection2();
}

void AMyController::UseEquippedItem2(const FInputActionValue& Value)
{
	float Input{Value.Get<float>()};
	ControlledPlayer2->UseEquipItem(FMath::FloorToInt32(Input));
}

void AMyController::UpdateDirection2()
{
	TMap<float , FVector> LatestInputs;

	if (bUpPressed2) {
		LatestInputs.Add(LastUpTime2 , FVector(1 , 0 , 0));
	}
	if (bDownPressed2) {
		LatestInputs.Add(LastDownTime2 , FVector(-1 , 0 , 0));
	}
	if (bRightPressed2) {
		LatestInputs.Add(LastRightTime2 , FVector(0 , 1 , 0));
	}
	if (bLeftPressed2) {
		LatestInputs.Add(LastLeftTime2 , FVector(0 , -1 , 0));
	}

	if (LatestInputs.Num() > 0) {
		TArray<float> Times;
		LatestInputs.GetKeys(Times);
		float MaxTime = FMath::Max(Times);
		ControlledPlayer2->Direction = LatestInputs[MaxTime];
	}
	else {
		ControlledPlayer2->Direction = FVector::ZeroVector;
	}
}

bool AMyController::CheckCollision2()
{
	FArrLocation PlayerArray{ControlledPlayer2->CheckLocation().X , ControlledPlayer2->CheckLocation().Y};
	FVector PlayerLocation{ControlledPlayer2->GetActorLocation()};
	float PlayerLength{PLAYER_SIZE};
	//LogUtils::Log("Player Location" , PlayerLocation.X , PlayerLocation.Y);

	FArrLocation CollisionCheckWallArray{
		PlayerArray.X - FMath::FloorToInt(ControlledPlayer2->Direction.X) ,
		PlayerArray.Y + FMath::FloorToInt(ControlledPlayer2->Direction.Y)
	};
	FVector WallLocation{
		MapGen->ArrayToWorldLocation(CollisionCheckWallArray)
	};

	uint8 WallType{static_cast<uint8>(MapGen->GameMap[CollisionCheckWallArray.X][CollisionCheckWallArray.Y])};
	if (ControlledPlayer2->CheckIfSpaceShip()) {
		if (WallType == EMapType::Blocking || WallType == EMapType::BalloonLoc) {
			if (FMath::Abs(ControlledPlayer2->Direction.X) == 1 && FMath::Abs(WallLocation.X - PlayerLocation.X) <
				PlayerLength) {
				return false;
			}
			if (FMath::Abs(ControlledPlayer2->Direction.Y) == 1 && FMath::Abs(WallLocation.Y - PlayerLocation.Y) <
				PlayerLength) {
				return false;
			}
		}
	}
	else {
		if (WallType == EMapType::Blocking || WallType == EMapType::Destroyable || WallType == EMapType::BalloonLoc) {
			if (FMath::Abs(ControlledPlayer2->Direction.X) == 1 && FMath::Abs(WallLocation.X - PlayerLocation.X) <
				PlayerLength) {
				return false;
			}
			if (FMath::Abs(ControlledPlayer2->Direction.Y) == 1 && FMath::Abs(WallLocation.Y - PlayerLocation.Y) <
				PlayerLength) {
				return false;
			}
		}
	}

	return true;
}

void AMyController::CheckItem2()
{
	if (ControlledPlayer2->CheckIfSpaceShip()) {
		return;
	}

	FArrLocation PlayerArray{ControlledPlayer2->CheckLocation().X , ControlledPlayer2->CheckLocation().Y};
	if (EMapType::BubbleItem <= MapGen->GameMap[PlayerArray.X][PlayerArray.Y] && MapGen->GameMap[PlayerArray.X][
		PlayerArray.Y] <= EMapType::TurtleItem) {
		ControlledPlayer2->GetItem(MapGen->ItemMap[PlayerArray.X][PlayerArray.Y]);

		MapGen->UpdateMapDestroyed(PlayerArray);
	}
}


///////////////////////////////////////////////////////////////


void AMyController::CheckEachCollision()
{
	if (ControlledPlayer->CheckLocation() == ControlledPlayer2->CheckLocation()) {
		if (ControlledPlayer->bIsTrapped && ControlledPlayer2->bIsTrapped) {
			return;
		}
		if (ControlledPlayer->bIsTrapped) {
			ControlledPlayer->Die();
			ControlledPlayer2->WinGame();
			return;
		}
		if (ControlledPlayer2->bIsTrapped) {
			ControlledPlayer2->Die();
			ControlledPlayer->WinGame();
			return;
		}
	}
}
