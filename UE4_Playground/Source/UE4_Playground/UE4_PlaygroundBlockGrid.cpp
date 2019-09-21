// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "UE4_PlaygroundBlockGrid.h"
#include "UE4_PlaygroundBlock.h"
#include "Components/TextRenderComponent.h"
#include "Engine/World.h"
#include "Character/MainCharacter.h"

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
	Width = 8;
	Height = 8;
	NumberBlocksClicked = 0;

	GridBitboard = 0;
	BlocksBitboard = 0;

}


void AUE4_PlaygroundBlockGrid::BeginPlay()
{
	Super::BeginPlay();	

	// Set the blocks
	
	BlocksBitboard = SetTileState(BlocksBitboard, 0, 0);
	BlocksBitboard = SetTileState(BlocksBitboard, 0, 2);
	BlocksBitboard = SetTileState(BlocksBitboard, 0, 4);
	BlocksBitboard = SetTileState(BlocksBitboard, 0, 5);
	BlocksBitboard = SetTileState(BlocksBitboard, 0, 6);
	BlocksBitboard = SetTileState(BlocksBitboard, 0, 7);

	BlocksBitboard = SetTileState(BlocksBitboard, 1, 0);
	BlocksBitboard = SetTileState(BlocksBitboard, 1, 5);
	BlocksBitboard = SetTileState(BlocksBitboard, 1, 7);


	BlocksBitboard = SetTileState(BlocksBitboard, 2, 3);
	BlocksBitboard = SetTileState(BlocksBitboard, 2, 7);

	BlocksBitboard = SetTileState(BlocksBitboard, 3, 0);
	BlocksBitboard = SetTileState(BlocksBitboard, 3, 4);
	BlocksBitboard = SetTileState(BlocksBitboard, 3, 7);

	BlocksBitboard = SetTileState(BlocksBitboard, 4, 0);

	BlocksBitboard = SetTileState(BlocksBitboard, 5, 5);
	BlocksBitboard = SetTileState(BlocksBitboard, 5, 6);

	BlocksBitboard = SetTileState(BlocksBitboard, 6, 1);
	BlocksBitboard = SetTileState(BlocksBitboard, 6, 4);
	BlocksBitboard = SetTileState(BlocksBitboard, 6, 5);

	BlocksBitboard = SetTileState(BlocksBitboard, 7, 0);
	BlocksBitboard = SetTileState(BlocksBitboard, 7, 3);
	BlocksBitboard = SetTileState(BlocksBitboard, 7, 5);
	BlocksBitboard = SetTileState(BlocksBitboard, 7, 6);
	BlocksBitboard = SetTileState(BlocksBitboard, 7, 7);

	UE_LOG(LogTemp, Warning, TEXT("AUE4_PlaygroundBlockGrid::BlocksBitboard %lu"), BlocksBitboard);

	GridBitboard = 0x0000000000000000000000000000000000000000000000000000000000000000;

	UE_LOG(LogTemp, Warning, TEXT("AUE4_PlaygroundBlockGrid::GridBitboard %lu"), GridBitboard);


	if (BlockClass == nullptr) return;


	float XOffset = 0.0f;
	float YOffset = 0.0f;

	for (int32 col = 0; col < Width; col++)	
	{
		for (int32 row = 0; row < Height; row++)
		{		
			XOffset = col * BlockSpacing;
			YOffset = row * BlockSpacing;
			const FVector BlockLocation = FVector(YOffset, XOffset, 0.0f) + GetActorLocation();

			AUE4_PlaygroundBlock* NewBlock = GetWorld()->SpawnActor<AUE4_PlaygroundBlock>(BlockClass, BlockLocation, FRotator(0, 0, 0));			

			// Tell the block about its owner
			if (NewBlock != nullptr)
			{
				NewBlock->OwningGrid = this;

				int32 id = (row * Width + col);;
				FString cellName = "Cell " + FString::FromInt(id) + " (" + FString::FromInt(row) + "," + FString::FromInt(col) + ")";

				NewBlock->SetActorLabel(cellName);

				// Check if it's a block
				if (GetTileState(BlocksBitboard, row, col))
				{
					UE_LOG(LogTemp, Warning, TEXT("GetTileState it's a block %d , %d"), row, col);

					NewBlock->SetBlockType(0);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("GetTileState it's NOT block %d , %d"), row, col);
				}
			}

			if ((row == 0) && (col == 1))
			{
				if (MainCharacterClass != nullptr)
				{
					
					const FVector MainPlayerPosition = FVector(XOffset, YOffset, 178.0f) + GetActorLocation();

					MainCharacter = GetWorld()->SpawnActor<AMainCharacter>(MainCharacterClass, MainPlayerPosition, FRotator(0, 0, 0));
				}
			}

			
		}
	}



}


void AUE4_PlaygroundBlockGrid::SetBlockClicked(class AUE4_PlaygroundBlock* Block)
{
	if (LastBlockClicked != Block)
	{
		NumberBlocksClicked++;
	}

	UE_LOG(LogTemp, Warning, TEXT("[AUE4_PlaygroundBlockGrid::SetBlockClicked] NumberBlocksClicked: %d"), NumberBlocksClicked);

	if (NumberBlocksClicked >= MaximunBlockClicks)
	{
		UE_LOG(LogTemp, Warning, TEXT("[AUE4_PlaygroundBlockGrid::SetBlockClicked] Reached max clicks"));
	}

	LastBlockClicked = Block;
}

void AUE4_PlaygroundBlockGrid::AddScore()
{
	// Increment score
	Score++;

	// Update text
	ScoreText->SetText(FText::Format(LOCTEXT("ScoreFmt", "Score: {0}"), FText::AsNumber(Score)));
}

#undef LOCTEXT_NAMESPACE


///// BIT BOARD ///////
// Sets the a cell state in the bitboard
int64_t AUE4_PlaygroundBlockGrid::SetTileState(const int64_t& bitBoard, const int32& row, const int32& column)
{
	UE_LOG(LogTemp, Warning, TEXT("AUE4_PlaygroundBlockGrid::SetTileState %l - ( %d , %d )"), bitBoard, row, column);

	// Set the bit in the correct position for the bitboard
	int64_t newBit = 1LL << (row * Width + column);

	newBit |= bitBoard;

	return (newBit);
}

// Returns the state of a specific row and column in a bitboard
bool AUE4_PlaygroundBlockGrid::GetTileState(const int64_t& bitBoard, const int32& row, const int32& column) const
{
	//UE_LOG(LogTemp, Warning, TEXT("AUE4_PlaygroundBlockGrid::GetTileState %ld - ( %d , %d )"), bitBoard, row, column);

	//long mask = 1uLL << (row * Width + column);
	int64_t mask = 1LL << (row * Width + column);

	return ((mask & bitBoard) != 0);
}

///// BIT BOARD ///////




