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

	if (GetWorld() == nullptr) return;

	UE_LOG(LogTemp, Warning, TEXT("[AUE4_PlaygroundPawn::BeginPlay] Looking for grids: "));

	for (TActorIterator<AUE4_PlaygroundBlockGrid> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{

		AUE4_PlaygroundBlockGrid *Grid = *ActorItr;

		if (Grid != nullptr)
		{
			CurrentGrid = Grid;
			UE_LOG(LogTemp, Warning, TEXT("[AUE4_PlaygroundPawn::BeginPlay] Found Grid: %s"), *Grid->GetName());
			return;
		}
	}

}

void AUE4_PlaygroundPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	/*if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		FVector Start, Dir, End;
		PC->DeprojectMousePositionToWorld(Start, Dir);
		End = Start + (Dir * 8000.0f);
		TraceForBlock(Start, End, false);		
	}*/
}

void AUE4_PlaygroundPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("TriggerClick", EInputEvent::IE_Pressed, this, &AUE4_PlaygroundPawn::TriggerClick);

	PlayerInputComponent->BindAction("Action", EInputEvent::IE_Pressed, this, &AUE4_PlaygroundPawn::StartAction);

	
}

void AUE4_PlaygroundPawn::StartAction()
{
	UE_LOG(LogTemp, Warning, TEXT("AUE4_PlaygroundPawn::StartAction"));

	if (CurrentGrid != nullptr)
	{
		CurrentGrid->StartAction();
	}
}

void AUE4_PlaygroundPawn::CalcCamera(float DeltaTime, struct FMinimalViewInfo& OutResult)
{
	Super::CalcCamera(DeltaTime, OutResult);

	OutResult.Rotation = FRotator(-90.0f, -90.0f, 0.0f);
}

void AUE4_PlaygroundPawn::TriggerClick()
{
	//UE_LOG(LogTemp, Warning, TEXT("[AUE4_PlaygroundPawn::TriggerClick] Called "));

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		FVector Start, Dir, End;
		PC->DeprojectMousePositionToWorld(Start, Dir);
		End = Start + (Dir * 8000.0f);

		FHitResult HitResult;
		GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility);
		//if (bDrawDebugHelpers)
		{
			DrawDebugLine(GetWorld(), Start, HitResult.Location, FColor::Red);
			DrawDebugSolidBox(GetWorld(), HitResult.Location, FVector(20.0f), FColor::Red);
		}
		if (HitResult.Actor.IsValid())
		{
			AUE4_PlaygroundBlock* HitBlock = Cast<AUE4_PlaygroundBlock>(HitResult.Actor.Get());
			if (HitBlock != nullptr)
			{
				UE_LOG(LogTemp, Warning, TEXT("HitBlock on block %s"), *HitBlock->GetActorLabel());

				//CurrentBlockFocus = HitBlock;

				if (CurrentGrid != nullptr)
				{
					CurrentGrid->HandleClickedOnBlock(HitBlock);
				}
			}
		}
	}





	if (CurrentBlockFocus == nullptr)
	{

		//UE_LOG(LogTemp, Warning, TEXT("[AUE4_PlaygroundPawn::TriggerClick] CurrentBlockFocus == nullptr "));
		//return;
	}

	if (CurrentGrid == nullptr)
	{

		//UE_LOG(LogTemp, Warning, TEXT("[AUE4_PlaygroundPawn::TriggerClick] CurrentGrid == nullptr "));
		//return;
	}

	//if ((CurrentBlockFocus == nullptr) || (CurrentGrid == nullptr)) return;

	//UE_LOG(LogTemp, Warning, TEXT("[AUE4_PlaygroundPawn::TriggerClick] Called not nulls"));

	//CurrentGrid->HandleClickedOnBlock(CurrentBlockFocus);



	/*HandleClickedOnBlock

	if (CurrentBlockFocus)
	{
		CurrentBlockFocus->HandleClicked();
	}*/
}

void AUE4_PlaygroundPawn::TraceForBlock(const FVector& Start, const FVector& End, bool bDrawDebugHelpers)
{
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility);
	if (bDrawDebugHelpers)
	{
		DrawDebugLine(GetWorld(), Start, HitResult.Location, FColor::Red);
		DrawDebugSolidBox(GetWorld(), HitResult.Location, FVector(20.0f), FColor::Red);
	}
	if (HitResult.Actor.IsValid())
	{
		AUE4_PlaygroundBlock* HitBlock = Cast<AUE4_PlaygroundBlock>(HitResult.Actor.Get());
		if (CurrentBlockFocus != HitBlock)
		{
			UE_LOG(LogTemp, Warning, TEXT("AUE4_PlaygroundPawn::TraceForBlock CurrentBlockFocus != HitBlock "));
			
			CurrentBlockFocus = HitBlock;
		}
	}
	else if (CurrentBlockFocus)
	{
		UE_LOG(LogTemp, Warning, TEXT("AUE4_PlaygroundPawn::TraceForBlock CurrentBlockFocus not null "));
		CurrentBlockFocus = nullptr;
	}
}