// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayGroundGameInstance.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "InGameUI.h"
#include "UE4_PlaygroundBlockGrid.h"
#include "EngineUtils.h"


UPlayGroundGameInstance::UPlayGroundGameInstance(const FObjectInitializer & ObjectInitializer)
{
	// Find the Widget and assigned to InGameUIClass
	static ConstructorHelpers::FClassFinder<UUserWidget> InGameUIBPClass(TEXT("/Game/Blueprints/WPI_InGame"));

	if (InGameUIBPClass.Class != nullptr)
	{
		InGameUIClass = InGameUIBPClass.Class; 
	}
}

void UPlayGroundGameInstance::Init()
{
	if (GetWorld() == nullptr) return;

	for (TActorIterator<AUE4_PlaygroundBlockGrid> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		AUE4_PlaygroundBlockGrid *Grid = *ActorItr;
		if (Grid != nullptr)
		{
			CurrentGrid = Grid;
			return;
		}
	}
}


// Interface IInGameMenuInterface

// Blueprint Callable Methods to create the the Menu
void UPlayGroundGameInstance::OnLoadInGameMenu()
{
	if (InGameUIClass == nullptr) return;

	InGameUI = CreateWidget<UInGameUI>(this, InGameUIClass);
	if (InGameUI == nullptr) return;

	InGameUI->AddToViewport();
	InGameUI->SetMenuInteraface(this);

	InGameUI->ShowInGameMessage("Click on a Tile to select a different one.\nPress the Start Path Button to make Taichi follow the path.");
}


void UPlayGroundGameInstance::OnStartGamePath()
{
	UE_LOG(LogTemp, Warning, TEXT("[UPlayGroundGameInstance::OnStartGamePath] "));

	if (CurrentGrid != nullptr)
	{
		CurrentGrid->OnStartPath();
	}
}

void UPlayGroundGameInstance::OnStartGame()
{
	UE_LOG(LogTemp, Warning, TEXT("[UPlayGroundGameInstance::OnStartGame] "));

	if (CurrentGrid != nullptr)
	{
		CurrentGrid->StartGame();
	}
}

// Interface IInGameMenuInterface

