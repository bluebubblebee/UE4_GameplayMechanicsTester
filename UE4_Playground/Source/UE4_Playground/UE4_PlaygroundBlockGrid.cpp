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
	TurnRightBlocksBitboard = 0;
	TurnLeftBlocksBitboard = 0;
	StraightBlocksBitboard = 0;

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

	StartRow = 0;
	StartCol = 1;
	CurrentRowMainChar = StartRow;
	CurrentColMainChar = StartCol;


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

					NewBlock->SetType(ETILETYPE::VE_BLOCKED);
				}
			}

			// Spawn main character
			if ((row == StartRow) && (col == StartCol))
			{
				if (MainCharacterClass != nullptr)
				{					
					const FVector MainPlayerPosition = FVector(YOffset, XOffset, 178.0f) + GetActorLocation();

					MainCharacter = GetWorld()->SpawnActor<AMainCharacter>(MainCharacterClass, MainPlayerPosition, FRotator(0, 0, 0));

					MainCharacter->OnEndMovement.AddDynamic(this, &AUE4_PlaygroundBlockGrid::OnCharacterEndOfMove);
					//MazeMapComponent->OnMazeMapGenerated.AddDynamic(this, &ANetTileMazeGameMode::OnMazeGenerated);

				}
			}
		}
	}
}

void AUE4_PlaygroundBlockGrid::HandleClickedOnBlock(class AUE4_PlaygroundBlock* Block)
{
	// Get type tyle clicked
	ETILETYPE tileType = GetTileType(Block->GetRow(), Block->GetCol());

	UE_LOG(LogTemp, Warning, TEXT("[AUE4_PlaygroundBlockGrid::StartAction] %s - %s "), *Block->GetActorLabel(), *GetEnumValueAsString<ETILETYPE>("ETILETYPE", tileType));
	
	// Prevent from clicking on Blocked tiles
	if (tileType == ETILETYPE::VE_BLOCKED) return;


	if (tileType == ETILETYPE::VE_BASE)
	{
		NumberBlocksClicked++;
	}

	
	// If it different of VE_BASE , Remove that type
	if (tileType != ETILETYPE::VE_BASE)
	{
		switch (tileType)
		{
		case ETILETYPE::VE_TURN_RIGHT:
			UE_LOG(LogTemp, Warning, TEXT("[AUE4_PlaygroundBlockGrid::StartAction] Remove: TurnRight "));
			TurnRightBlocksBitboard = RemoveTileState(TurnRightBlocksBitboard, Block->GetRow(), Block->GetCol());

			break;
		case ETILETYPE::VE_TURN_LEFT:
			
			TurnLeftBlocksBitboard = RemoveTileState(TurnLeftBlocksBitboard, Block->GetRow(), Block->GetCol());

			UE_LOG(LogTemp, Warning, TEXT("[AUE4_PlaygroundBlockGrid::StartAction] Remove: TurnLeft - "));

			break;
		case ETILETYPE::VE_STRAIGHT:
			
			StraightBlocksBitboard = RemoveTileState(StraightBlocksBitboard, Block->GetRow(), Block->GetCol());

			UE_LOG(LogTemp, Warning, TEXT("[AUE4_PlaygroundBlockGrid::StartAction] Remove: Straight: "));

			

			break;
		}
	}


	ETILETYPE nextType = GetNextTileType(tileType);
	Block->SetType(nextType);

	UE_LOG(LogTemp, Warning, TEXT("[AUE4_PlaygroundBlockGrid::StartAction] nextType: %s "), *GetEnumValueAsString<ETILETYPE>("ETILETYPE", nextType));

	switch (nextType)
	{
	case ETILETYPE::VE_TURN_RIGHT:
		UE_LOG(LogTemp, Warning, TEXT("[AUE4_PlaygroundBlockGrid::StartAction] Add: TurnRight "));
		TurnRightBlocksBitboard = SetTileState(TurnRightBlocksBitboard, Block->GetRow(), Block->GetCol());
		break;

		case ETILETYPE::VE_TURN_LEFT:
		UE_LOG(LogTemp, Warning, TEXT("[AUE4_PlaygroundBlockGrid::StartAction] Add: TurnLeft "));
		TurnLeftBlocksBitboard = SetTileState(TurnLeftBlocksBitboard, Block->GetRow(), Block->GetCol());
		break;

	case ETILETYPE::VE_STRAIGHT:
		UE_LOG(LogTemp, Warning, TEXT("[AUE4_PlaygroundBlockGrid::StartAction] Add: Straight "));
		StraightBlocksBitboard = SetTileState(StraightBlocksBitboard, Block->GetRow(), Block->GetCol());
		break;

	}
}


void AUE4_PlaygroundBlockGrid::SetBlockClicked(class AUE4_PlaygroundBlock* Block)
{
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
	CurrentRowMainChar += 1;
	ETILETYPE type = GetTileType(CurrentColMainChar, CurrentRowMainChar);

	//EUsesEnum UseEnumValue = EUsesEnum::UseEnum_Good;
	//UE_LOG(LogSomething, Log, TEXT("UseEnumValue as string : %s"), *GETENUMSTRING("EUsesEnum", UseEnumValue));

	FString typeName = GetEnumValueAsString<ETILETYPE>("ETILETYPE", type);	

	UE_LOG(LogTemp, Warning, TEXT("[AUE4_PlaygroundBlockGrid::StartAction] Next Tyle: %d %s "), type, *typeName);


	float xPostion = CurrentColMainChar * BlockSpacing;
	float yPostion = CurrentRowMainChar * BlockSpacing;



	const FVector NewPosition = FVector(yPostion, xPostion, 178.0f) + GetActorLocation();
	
	MainCharacter->MoveToPosition(NewPosition);

	//MainCharacter->SetActorLocation(MainPlayerPosition);



}

void AUE4_PlaygroundBlockGrid::OnCharacterEndOfMove()
{
	UE_LOG(LogTemp, Warning, TEXT("[AUE4_PlaygroundBlockGrid::OnCharacterEndOfMove]"));

}



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
	// toggle the attribute value on and off (XOR, exclusive or)
	 /*a | b | a ^ b
							--|---|------
							0 | 0 | 0
							0 | 1 | 1
							1 | 0 | 1
							1 | 1 | 0*/
	int64_t newBit = 1LL << (row * Width + column);

	newBit ^= bitBoard;

	return (newBit);
}

// Returns the state of a specific row and column in a bitboard
bool AUE4_PlaygroundBlockGrid::GetTileState(const int64_t& bitBoard, const int32& row, const int32& column) const
{
	//UE_LOG(LogTemp, Warning, TEXT("AUE4_PlaygroundBlockGrid::GetTileState %ld - ( %d , %d )"), bitBoard, row, column);
	int64_t mask = 1LL << (row * Width + column);

	return ((mask & bitBoard) != 0);
}

FString AUE4_PlaygroundBlockGrid::BitsToString(const int64_t& bitBoard)
{
	FString Bits;
	Bits.Reserve(FMath::FloorToInt(FMath::Sqrt(bitBoard))); //Reserveing space for string in memory FString won't rellocate string multiple times in process
	//We multiply by 2 until it's bigger then n where every bit will be 0 after that
	for (int i = 1; i < bitBoard; (i * 2))
	{
		if (bitBoard & i) Bits.Append("1");
		else Bits.Append("0");
	}

	return Bits.Reverse(); //We need to reverse bit order, so smallest is last
}


ETILETYPE AUE4_PlaygroundBlockGrid::GetTileType(const int32 row, const int32 column)
{
	ETILETYPE tile = ETILETYPE::VE_BASE;

	if ((row == StartRow) && (column == StartCol)) return ETILETYPE::VE_START;

	if ((row == EndRow) && (column == EndCol)) return ETILETYPE::VE_END;

	if (GetTileState(BlocksBitboard, row, column))
	{
		UE_LOG(LogTemp, Warning, TEXT("[GetTileType.BlocksBitboard]  "));
		tile = ETILETYPE::VE_BLOCKED;
	}
	else if (GetTileState(TurnLeftBlocksBitboard, row, column))
	{
		tile = ETILETYPE::VE_TURN_LEFT;

		UE_LOG(LogTemp, Warning, TEXT("[GetTileType.TurnLeft]  "));
	}
	

	else if (GetTileState(StraightBlocksBitboard, row, column))
	{
		tile = ETILETYPE::VE_STRAIGHT;
		UE_LOG(LogTemp, Warning, TEXT("[GetTileType.Staright]  "));
	}

	else if (GetTileState(TurnRightBlocksBitboard, row, column))
	{
		tile = ETILETYPE::VE_TURN_RIGHT;
		UE_LOG(LogTemp, Warning, TEXT("[GetTileType.TurnRight]  "));
	}

	return tile;
}

ETILETYPE AUE4_PlaygroundBlockGrid::GetNextTileType(ETILETYPE currentType)
{
	int32 nextType = (int32)currentType;
	 
	int32 lastType = (int32)ETILETYPE::VE_STRAIGHT;

	nextType += 1;

	if (nextType > lastType)
	{
		nextType = (int32)ETILETYPE::VE_TURN_RIGHT; // First one again
	}

	return (ETILETYPE)nextType;
}

///// BIT BOARD ///////

#undef LOCTEXT_NAMESPACE



