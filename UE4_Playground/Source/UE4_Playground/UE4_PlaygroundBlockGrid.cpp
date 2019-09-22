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

	//UE_LOG(LogTemp, Warning, TEXT("AUE4_PlaygroundBlockGrid::BlocksBitboard %lu"), BlocksBitboard);

	//GridBitboard = 0x0000000000000000000000000000000000000000000000000000000000000000;

	//UE_LOG(LogTemp, Warning, TEXT("AUE4_PlaygroundBlockGrid::GridBitboard %lu"), GridBitboard);


	if (BlockClass == nullptr) return;

	CurrentRowMainChar = 0;
	CurrentColMainChar = 1;


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
				NewBlock->SetRow(row);
				NewBlock->SetCol(col);

				// Check if it's a block
				if (GetTileState(BlocksBitboard, row, col))
				{
					UE_LOG(LogTemp, Warning, TEXT("GetTileState it's a block %d , %d"), row, col);

					NewBlock->SetBlockType(ETILETYPE::VE_BLOCKED);
				}
			}

			// Spawn main character
			if ((row == CurrentRowMainChar) && (col == CurrentColMainChar))
			{
				if (MainCharacterClass != nullptr)
				{					
					const FVector MainPlayerPosition = FVector(YOffset, XOffset, 178.0f) + GetActorLocation();

					MainCharacter = GetWorld()->SpawnActor<AMainCharacter>(MainCharacterClass, MainPlayerPosition, FRotator(0, 0, 0));
				}
			}
		}
	}
}


void AUE4_PlaygroundBlockGrid::SetBlockClicked(class AUE4_PlaygroundBlock* Block)
{

	if (!Block->IsActive())
	{
		Block->SetActive(true);

		NumberBlocksClicked++;
	}	

	LastBlockClicked = Block;

	UE_LOG(LogTemp, Warning, TEXT("[AUE4_PlaygroundBlockGrid::SetBlockClicked] %s: Type %d - (%d, %d) ->  NumberBlocksClicked: %d"), *Block->GetActorLabel(), Block->GetType(), Block->GetRow(), Block->GetCol(), NumberBlocksClicked);

	if (NumberBlocksClicked >= MaximunBlockClicks)
	{
		//UE_LOG(LogTemp, Warning, TEXT("[AUE4_PlaygroundBlockGrid::SetBlockClicked] Reached max clicks"));
	}

	// Check billboard
	
	// Check tiles 
	// Check on that position there was a tile
	if (GetTileState(TurnRightBlocksBitboard, Block->GetRow(), Block->GetCol()))
	{
		//UE_LOG(LogTemp, Warning, TEXT("[AUE4_PlaygroundBlockGrid::SetBlockClicked] %s (%d ,%d) = TurnRightBlocksBitboard"), *Block->GetActorLabel(), Block->GetRow(), Block->GetCol());
		
		// Removed state
		TurnRightBlocksBitboard = RemoveTileState(TurnRightBlocksBitboard, Block->GetRow(), Block->GetCol());
	}

	if (GetTileState(TurnLeftBlocksBitboard, Block->GetRow(), Block->GetCol()))
	{
		//UE_LOG(LogTemp, Warning, TEXT("[AUE4_PlaygroundBlockGrid::SetBlockClicked] %s (%d ,%d) = TurnLeftBlocksBitboard"), *Block->GetActorLabel(), Block->GetRow(), Block->GetCol());
		TurnLeftBlocksBitboard = RemoveTileState(TurnLeftBlocksBitboard, Block->GetRow(), Block->GetCol());
	}


	if (GetTileState(StraightBlocksBitboard, Block->GetRow(), Block->GetCol()))
	{
		//UE_LOG(LogTemp, Warning, TEXT("[AUE4_PlaygroundBlockGrid::SetBlockClicked] %s (%d ,%d) = StraightBlocksBitboard"), *Block->GetActorLabel(), Block->GetRow(), Block->GetCol());
		StraightBlocksBitboard = RemoveTileState(StraightBlocksBitboard, Block->GetRow(), Block->GetCol());
	}



	switch (Block->GetType())
	{
	case ETILETYPE::VE_TURN_RIGHT:
		
		// Set TurnRight for this cell
		TurnRightBlocksBitboard = SetTileState(TurnRightBlocksBitboard, Block->GetRow(), Block->GetCol());
		break;
	case ETILETYPE::VE_TURN_LEFT:
		TurnLeftBlocksBitboard = SetTileState(TurnLeftBlocksBitboard, Block->GetRow(), Block->GetCol());
		break;
	case ETILETYPE::VE_STRAIGHT:
		StraightBlocksBitboard = SetTileState(StraightBlocksBitboard, Block->GetRow(), Block->GetCol());
		break;
	}
}

void AUE4_PlaygroundBlockGrid::AddScore()
{
	// Increment score
	Score++;

	// Update text
	ScoreText->SetText(FText::Format(LOCTEXT("ScoreFmt", "Score: {0}"), FText::AsNumber(Score)));
}

void AUE4_PlaygroundBlockGrid::StartAction()
{
	if (MainCharacter == nullptr) return;

	//UE_LOG(LogTemp, Warning, TEXT("[AUE4_PlaygroundBlockGrid::StartAction]"));

	// Move to next direction

	// Check next cell
	
	// If first time, character is looking forward
	//CurrentRowMainChar++;


	ETILETYPE type = GetTileType(CurrentColMainChar, CurrentRowMainChar +1);

	//EUsesEnum UseEnumValue = EUsesEnum::UseEnum_Good;
	//UE_LOG(LogSomething, Log, TEXT("UseEnumValue as string : %s"), *GETENUMSTRING("EUsesEnum", UseEnumValue));

	FString typeName = GetEnumValueAsString<ETILETYPE>("ETILETYPE", type);	

	UE_LOG(LogTemp, Warning, TEXT("[AUE4_PlaygroundBlockGrid::StartAction] Next Tyle: %d %s "), type, *typeName);


	float xPostion = CurrentColMainChar * BlockSpacing;
	float yPostion = CurrentRowMainChar * BlockSpacing;


	const FVector MainPlayerPosition = FVector(yPostion, xPostion, 178.0f) + GetActorLocation();
	//MainCharacter->SetActorLocation(MainPlayerPosition);



}

#undef LOCTEXT_NAMESPACE


///// BIT BOARD ///////
// Sets the a cell state in the bitboard
int64_t AUE4_PlaygroundBlockGrid::SetTileState(const int64_t& bitBoard, const int32& row, const int32& column)
{
	//UE_LOG(LogTemp, Warning, TEXT("AUE4_PlaygroundBlockGrid::SetTileState %l - ( %d , %d )"), bitBoard, row, column);

	// Set the bit in the correct position for the bitboard
	int64_t newBit = 1LL << (row * Width + column);

	newBit |= bitBoard;

	return (newBit);
}

int64_t AUE4_PlaygroundBlockGrid::RemoveTileState(const int64_t& bitBoard, const int32& row, const int32& column)
{
	// Set the bit in the correct position for the bitboard
	int64_t newBit = 1LL >> (row * Width + column);

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


ETILETYPE AUE4_PlaygroundBlockGrid::GetTileType(const int32 row, const int32 column)
{
	if (GetTileState(BlocksBitboard, row, column))
	{
		return ETILETYPE::VE_BLOCKED;
	}

	if (GetTileState(TurnRightBlocksBitboard, row, column))
	{
		return ETILETYPE::VE_TURN_RIGHT;
	}

	if (GetTileState(TurnLeftBlocksBitboard, row, column))
	{
		return ETILETYPE::VE_TURN_LEFT;
	}


	if (GetTileState(StraightBlocksBitboard, row, column))
	{
		return ETILETYPE::VE_STRAIGHT;
	}

	return ETILETYPE::VE_NONE;
}

///// BIT BOARD ///////




