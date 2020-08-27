// Copyright Epic Games, Inc. All Rights Reserved.

#include "HellGameGameMode.h"
#include "HellGameHUD.h"
#include "HellGameCharacter.h"
#include "UObject/ConstructorHelpers.h"

AHellGameGameMode::AHellGameGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	PlayerControllerClass = APlayerController::StaticClass();
	// use our custom HUD class
	HUDClass = AHellGameHUD::StaticClass();
}
