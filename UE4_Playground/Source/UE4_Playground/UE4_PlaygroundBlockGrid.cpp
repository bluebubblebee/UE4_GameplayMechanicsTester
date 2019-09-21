// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "UE4_PlaygroundBlockGrid.h"
#include "UE4_PlaygroundBlock.h"
#include "Components/TextRenderComponent.h"
#include "Engine/World.h"

#define LOCTEXT_NAMESPACE "PuzzleBlockGrid"

AUE4_PlaygroundBlockGrid::AUE4_PlaygroundBlockGrid()
{
	// Create dummy root scene component
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = DummyRoot;

	// Create static mesh component
	ScoreText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("ScoreText0"));
	ScoreText->SetRelativeLocation(FVector(200.f,0.f,0.f));
	ScoreText->SetRelativeRotation(FRotator(90.f,0.f,0.f));
	ScoreText->SetText(FText::Format(LOCTEXT("ScoreFmt", "Score: {0}"), FText::AsNumber(0)));
	ScoreText->SetupAttachment(DummyRoot);

	MaximunBlockClicks = 10;
	BlockSpacing = 265.0f;
	Size = 8;
	NumberBlocksClicked = 0;

}


void AUE4_PlaygroundBlockGrid::BeginPlay()
{
	Super::BeginPlay();

	if (BlockClass == nullptr) return;

	const int32 NumBlocks = Size * Size;

	for(int32 BlockIndex=0; BlockIndex < NumBlocks; BlockIndex++)
	{
		const float XOffset = (BlockIndex/Size) * BlockSpacing; // Divide by dimension
		const float YOffset = (BlockIndex%Size) * BlockSpacing; // Modulo gives remainder

		// Make position vector, offset from Grid location
		const FVector BlockLocation = FVector(XOffset, YOffset, 0.f) + GetActorLocation();

		AUE4_PlaygroundBlock* NewBlock = GetWorld()->SpawnActor<AUE4_PlaygroundBlock>(BlockClass, BlockLocation, FRotator(0, 0, 0));	

		// Tell the block about its owner
		if (NewBlock != nullptr)
		{
			NewBlock->OwningGrid = this;
		}
	}
}


void AUE4_PlaygroundBlockGrid::SetBlockClicked(class AUE4_PlaygroundBlock* Block)
{
	if (lastBlockClicked != Block)
	{
		NumberBlocksClicked++;
	}

	UE_LOG(LogTemp, Warning, TEXT("[AUE4_PlaygroundBlockGrid::SetBlockClicked] NumberBlocksClicked: %d"), NumberBlocksClicked);

	if (NumberBlocksClicked >= MaximunBlockClicks)
	{
		UE_LOG(LogTemp, Warning, TEXT("[AUE4_PlaygroundBlockGrid::SetBlockClicked] Reached max clicks"));
	}

	lastBlockClicked = Block;
}

void AUE4_PlaygroundBlockGrid::AddScore()
{
	// Increment score
	Score++;

	// Update text
	ScoreText->SetText(FText::Format(LOCTEXT("ScoreFmt", "Score: {0}"), FText::AsNumber(Score)));
}

#undef LOCTEXT_NAMESPACE
