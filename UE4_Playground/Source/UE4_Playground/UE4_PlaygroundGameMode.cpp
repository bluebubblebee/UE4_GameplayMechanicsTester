// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "UE4_PlaygroundGameMode.h"
#include "UE4_PlaygroundPlayerController.h"
#include "UE4_PlaygroundPawn.h"

AUE4_PlaygroundGameMode::AUE4_PlaygroundGameMode()
{
	// no pawn by default
	DefaultPawnClass = AUE4_PlaygroundPawn::StaticClass();
	// use our own player controller class
	PlayerControllerClass = AUE4_PlaygroundPlayerController::StaticClass();
}
