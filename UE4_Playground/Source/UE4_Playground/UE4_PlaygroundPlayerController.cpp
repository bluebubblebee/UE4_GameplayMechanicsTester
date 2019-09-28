// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "UE4_PlaygroundPlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "InGameUI.h"

AUE4_PlaygroundPlayerController::AUE4_PlaygroundPlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableTouchEvents = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs; 

	// Find the Widget and assigned to InGameUIClass
	static ConstructorHelpers::FClassFinder<UUserWidget> InGameUIBPClass(TEXT("/Game/Blueprints/WPI_InGame"));

	if (InGameUIBPClass.Class != nullptr)
	{
		InGameUIClass = InGameUIBPClass.Class;
	}
}


void AUE4_PlaygroundPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Create the widget and add to the viewport
	if (InGameUIClass != nullptr)
	{
		InGameUI = CreateWidget<UInGameUI>(this, InGameUIClass);

		if (InGameUI != nullptr)
		{
			InGameUI->AddToViewport();

			//InGameUI->UpdateInGameMessage("CLICK ON THE TILES TO CHANGE THEM. WHEN READY, PRESS THE PLAY BUTTON TO MAKE TAICHI FOLLOW THE PATH");
		}
	}
}

