// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameModeBase.h"
#include "AppleCharacter.h"
#include "MyController.h"

AMyGameModeBase::AMyGameModeBase()
{
	PlayerControllerClass = AMyController::StaticClass();
	DefaultPawnClass = AAppleCharacter::StaticClass();
}
