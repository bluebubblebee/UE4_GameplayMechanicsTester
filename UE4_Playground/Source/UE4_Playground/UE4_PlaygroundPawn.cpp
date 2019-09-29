// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "UE4_PlaygroundPawn.h"
#include "UE4_PlaygroundBlock.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"
#include "UE4_PlaygroundBlockGrid.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"

AUE4_PlaygroundPawn::AUE4_PlaygroundPawn(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer)
{
	AutoPossessPlayer = EAutoReceiveInput::Player0;	
}


void AUE4_PlaygroundPawn::BeginPlay()
{

	Super::BeginPlay();
}

void AUE4_PlaygroundPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AUE4_PlaygroundPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}


