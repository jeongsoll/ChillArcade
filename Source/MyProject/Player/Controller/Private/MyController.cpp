// Fill out your copyright notice in the Description page of Project Settings.


#include "MyController.h"
#include "InputMappingContext.h" /*UInputMappingContext*/
#include "EnhancedInputSubsystems.h" /*UEnhancedInputLocalPlayerSubsystem*/
#include "EnhancedInputComponent.h" /*UEnhancedInputComponent*/

AMyController::AMyController()
{
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> ImcPlayer
		(TEXT("/Game/Player/Input/IMC_Player.IMC_Player"));
	if (ImcPlayer.Succeeded()) {
		InputMappingContext = ImcPlayer.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionMove
		(TEXT("/Game/Player/Input/IA_Move.IA_Move"));
	if (InputActionMove.Succeeded()) {
		MoveAction = InputActionMove.Object;
	}
}

void AMyController::BeginPlay()
{
	Super::BeginPlay();
	if (UEnhancedInputLocalPlayerSubsystem* subsystem{ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer())}) {
		subsystem->AddMappingContext(InputMappingContext, 0);
	}
}

void AMyController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	if (UEnhancedInputComponent* EnhancedInputComponent{CastChecked<UEnhancedInputComponent>(InputComponent)}) {
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyController::MovePlayer);
	}
}

void AMyController::MovePlayer(const struct FInputActionValue& Value)
{
	const FVector2D Dir{Value.Get<FVector2D>()};
	if (APawn* ControlledPawn{GetPawn()}) {
		FVector Direction{};
		Direction.X = Dir.X;
		Direction.Y = Dir.Y;
		ControlledPawn->AddMovementInput(Direction);
		ControlledPawn->AddMovementInput(Direction);
	}
}
