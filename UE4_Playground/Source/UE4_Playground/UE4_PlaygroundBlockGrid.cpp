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

	MaximunTileClicks = 10;
	TileSpacing = 265.0f;
	Width = 8;
	Height = 8;
	NumberTileClicked = 0;

	StartRow = 0;
	StartCol = 1;

	EndRow = 6;
	EndCol = 6;

	BlockedTilesBitboard = 0;
	TurnRightTilesBitboard = 0;
	TurnLeftTilesBitboard = 0;
	StraightTilesBitboard = 0;
}


void AUE4_PlaygroundBlockGrid::BeginPlay()
{
	Super::BeginPlay();	

	// Set the blocks
	
	BlockedTilesBitboard = ToggleTile(BlockedTilesBitboard, 0, 0);
	BlockedTilesBitboard = ToggleTile(BlockedTilesBitboard, 0, 2);
	BlockedTilesBitboard = ToggleTile(BlockedTilesBitboard, 0, 4);
	BlockedTilesBitboard = ToggleTile(BlockedTilesBitboard, 0, 5);
	BlockedTilesBitboard = ToggleTile(BlockedTilesBitboard, 0, 6);
	BlockedTilesBitboard = ToggleTile(BlockedTilesBitboard, 0, 7);

	BlockedTilesBitboard = ToggleTile(BlockedTilesBitboard, 1, 0);
	BlockedTilesBitboard = ToggleTile(BlockedTilesBitboard, 1, 5);
	BlockedTilesBitboard = ToggleTile(BlockedTilesBitboard, 1, 7);


	BlockedTilesBitboard = ToggleTile(BlockedTilesBitboard, 2, 3);
	BlockedTilesBitboard = ToggleTile(BlockedTilesBitboard, 2, 7);

	BlockedTilesBitboard = ToggleTile(BlockedTilesBitboard, 3, 0);
	BlockedTilesBitboard = ToggleTile(BlockedTilesBitboard, 3, 4);
	BlockedTilesBitboard = ToggleTile(BlockedTilesBitboard, 3, 7);

	BlockedTilesBitboard = ToggleTile(BlockedTilesBitboard, 4, 0);

	BlockedTilesBitboard = ToggleTile(BlockedTilesBitboard, 5, 5);
	BlockedTilesBitboard = ToggleTile(BlockedTilesBitboard, 5, 6);

	BlockedTilesBitboard = ToggleTile(BlockedTilesBitboard, 6, 1);
	BlockedTilesBitboard = ToggleTile(BlockedTilesBitboard, 6, 4);
	BlockedTilesBitboard = ToggleTile(BlockedTilesBitboard, 6, 5);

	BlockedTilesBitboard = ToggleTile(BlockedTilesBitboard, 7, 0);
	BlockedTilesBitboard = ToggleTile(BlockedTilesBitboard, 7, 3);
	BlockedTilesBitboard = ToggleTile(BlockedTilesBitboard, 7, 5);
	BlockedTilesBitboard = ToggleTile(BlockedTilesBitboard, 7, 6);
	BlockedTilesBitboard = ToggleTile(BlockedTilesBitboard, 7, 7);

	// Spawn tiles
	if (TileClass == nullptr) return;
	
	CurrentRowMainChar = StartRow;
	CurrentColMainChar = StartCol;

	float XOffset = 0.0f;
	float YOffset = 0.0f;

	for (int32 col = 0; col < Width; col++)	
	{
		for (int32 row = 0; row < Height; row++)
		{		
			XOffset = col * TileSpacing;
			YOffset = row * TileSpacing;
			const FVector TileLocation = FVector(YOffset, XOffset, 0.0f) + GetActorLocation();

			AUE4_PlaygroundBlock* NewTile = GetWorld()->SpawnActor<AUE4_PlaygroundBlock>(TileClass, TileLocation, FRotator(0, 0, 0));

			// Tell the block about its owner
			if (NewTile != nullptr)
			{
				NewTile->OwningGrid = this;

				int32 index = (row * Width + col);;
				FString cellName = "Cell " + FString::FromInt(index) + " (" + FString::FromInt(row) + "," + FString::FromInt(col) + ")";

				NewTile->SetActorLabel(cellName);
				NewTile->SetRow(row);
				NewTile->SetCol(col);

				// Check if it's a block
				if (GetTileState(BlockedTilesBitboard, row, col))
				{
					NewTile->SetType(ETILETYPE::VE_BLOCKED);
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

				}

				if (NewTile != nullptr)
				{
					NewTile->SetType(ETILETYPE::VE_START);
				}
			}

			if ((row == EndRow) && (col == EndCol))
			{
				if (NewTile != nullptr)
				{
					NewTile->SetType(ETILETYPE::VE_END);
				}
			}
		}
	}

	currentTile = ETILETYPE::VE_BASE;
	currentDirection = EDIRECTION::VE_NONE;
	bIsInputLocked = false;
}


void AUE4_PlaygroundBlockGrid::HandleClickedOnTile(class AUE4_PlaygroundBlock* Tile)
{
	// If max block clicked return
	if (NumberTileClicked >= MaximunTileClicks) return;

	// Prevent from clicking on Start,EndRow and where the character is
	if ((Tile->GetRow() == StartRow) && (Tile->GetCol() == StartCol)) return;

	if ((Tile->GetRow() == EndRow) && (Tile->GetCol() == EndCol)) return;

	if ((Tile->GetRow() == CurrentRowMainChar) && (Tile->GetCol() == CurrentColMainChar)) return;

	// Get type tyle clicked
	ETILETYPE tileType = GetTileType(Tile->GetRow(), Tile->GetCol());

	// Prevent from clicking on Blocked tiles
	if (tileType == ETILETYPE::VE_BLOCKED) return;

	if (tileType == ETILETYPE::VE_BASE)
	{
		NumberTileClicked++;
	}
	
	// If it different of VE_BASE , Remove that type
	if (tileType != ETILETYPE::VE_BASE)
	{
		switch (tileType)
		{
		case ETILETYPE::VE_TURN_RIGHT:
			TurnRightTilesBitboard = ToggleTile(TurnRightTilesBitboard, Tile->GetRow(), Tile->GetCol());

			break;
		case ETILETYPE::VE_TURN_LEFT:
			
			TurnLeftTilesBitboard = ToggleTile(TurnLeftTilesBitboard, Tile->GetRow(), Tile->GetCol());

			break;
		case ETILETYPE::VE_STRAIGHT:
			
			StraightTilesBitboard = ToggleTile(StraightTilesBitboard, Tile->GetRow(), Tile->GetCol());

			break;
		}
	}

	// Get next type of tile and toggle it
	ETILETYPE nextType = GetNextTileType(tileType);
	Tile->SetType(nextType);

	switch (nextType)
	{
	case ETILETYPE::VE_TURN_RIGHT:
		TurnRightTilesBitboard = ToggleTile(TurnRightTilesBitboard, Tile->GetRow(), Tile->GetCol());
		break;

		case ETILETYPE::VE_TURN_LEFT:
		TurnLeftTilesBitboard = ToggleTile(TurnLeftTilesBitboard, Tile->GetRow(), Tile->GetCol());
		break;

	case ETILETYPE::VE_STRAIGHT:
		StraightTilesBitboard = ToggleTile(StraightTilesBitboard, Tile->GetRow(), Tile->GetCol());
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


///// MAIN CHARACTER: Path ///////
void AUE4_PlaygroundBlockGrid::StartAction()
{
	if (bIsInputLocked) return;

	if (MainCharacter == nullptr) return;  	 

	// Start Path
	CurrentRowMainChar += 1;

	currentDirection = EDIRECTION::VE_UP;

	currentTile = GetTileType(CurrentRowMainChar, CurrentColMainChar);

	MainCharacter->MoveToDirection(currentDirection, TileSpacing);	

	bIsInputLocked = true;
}

void AUE4_PlaygroundBlockGrid::OnCharacterEndOfMove()
{
	UE_LOG(LogTemp, Warning, TEXT("[AUE4_PlaygroundBlockGrid::OnCharacterEndOfMove] - (%d, %d): CurrentTile: %s - lastDirection: %s"), CurrentRowMainChar, CurrentColMainChar, *GetEnumValueAsString<ETILETYPE>("ETILETYPE", currentTile), *GetEnumValueAsString<EDIRECTION>("EDIRECTION", currentDirection));

	EDIRECTION dirCharacter = currentDirection;

	if (currentTile == ETILETYPE::VE_STRAIGHT)
	{
		

		if (currentDirection == EDIRECTION::VE_UP)
		{
			CurrentRowMainChar += 1;

			dirCharacter = EDIRECTION::VE_UP;
		}
		else if (currentDirection == EDIRECTION::VE_DOWN)
		{
			CurrentRowMainChar -= 1;

			dirCharacter = EDIRECTION::VE_DOWN;
		}
		else if (currentDirection == EDIRECTION::VE_LEFT)
		{
			CurrentColMainChar -= 1;
			dirCharacter = EDIRECTION::VE_LEFT;
		}
		else if (currentDirection == EDIRECTION::VE_RIGHT)
		{
			CurrentColMainChar += 1;
			dirCharacter = EDIRECTION::VE_RIGHT;
		}
	}

	else if (currentTile == ETILETYPE::VE_TURN_RIGHT)
	{
		dirCharacter = EDIRECTION::VE_LEFT;

		// Change direction
		if (currentDirection == EDIRECTION::VE_UP)
		{
			CurrentColMainChar += 1;
			currentDirection = EDIRECTION::VE_RIGHT;

			dirCharacter = EDIRECTION::VE_RIGHT;

		}
		else if (currentDirection == EDIRECTION::VE_LEFT)
		{
			CurrentRowMainChar += 1;
			currentDirection = EDIRECTION::VE_UP;

			dirCharacter = EDIRECTION::VE_UP;
		}
		else if (currentDirection == EDIRECTION::VE_RIGHT)
		{
			CurrentRowMainChar -= 1;
			currentDirection = EDIRECTION::VE_DOWN;

			dirCharacter = EDIRECTION::VE_DOWN;
		}

		MainCharacter->RotateToDirection(EDIRECTION::VE_RIGHT);
	}

	else if (currentTile == ETILETYPE::VE_TURN_LEFT)
	{
		dirCharacter = EDIRECTION::VE_LEFT;

		// Change direction
		if (currentDirection == EDIRECTION::VE_RIGHT)
		{
			CurrentRowMainChar += 1;
			currentDirection = EDIRECTION::VE_UP;

			dirCharacter = EDIRECTION::VE_UP;

		}
		else if (currentDirection == EDIRECTION::VE_UP)
		{
			CurrentColMainChar -= 1;
			currentDirection = EDIRECTION::VE_LEFT;

			dirCharacter = EDIRECTION::VE_LEFT;
		}

		MainCharacter->RotateToDirection(EDIRECTION::VE_LEFT);
	}


	ETILETYPE tempTile = GetTileType(CurrentRowMainChar, CurrentColMainChar);
	if ((tempTile == ETILETYPE::VE_BASE) || (tempTile == ETILETYPE::VE_BLOCKED) || (tempTile == ETILETYPE::VE_BASE))
	{
		UE_LOG(LogTemp, Warning, TEXT(" CANT'T MOVE ANY LONGER:  %s "), *GetEnumValueAsString<ETILETYPE>("ETILETYPE", tempTile));
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("  "));

		UE_LOG(LogTemp, Warning, TEXT(" Keep going (tempTile):  %s "), *GetEnumValueAsString<ETILETYPE>("ETILETYPE", tempTile));

		MainCharacter->MoveToDirection(dirCharacter, TileSpacing);

		if (tempTile == ETILETYPE::VE_END)
		{
			UE_LOG(LogTemp, Warning, TEXT(" REACHED END!! "));
		}
	}

}
///// MAIN CHARACTER: Path ///////


///// BIT BOARD ///////

//  Toggles 1 bit with XOR Bitwise operation (^)  to a given bitboard, in a ceratan row and column
int64_t AUE4_PlaygroundBlockGrid::ToggleTile(const int64_t& bitBoard, const int32& row, const int32& column)
{
	// Set the bit in the correct position for the bitboard 
	// toggle the attribute value on and off (XOR, exclusive or)
	 /*   
	      a | b | a ^ b
		  --|---|------
		  0 | 0 |  0
		  0 | 1 |  1
		  1 | 0 |  1
		  1 | 1 |  0

	*/

	int64_t newBit = 1LL << (row * Width + column);

	newBit ^= bitBoard;

	return (newBit);
}


// Returns the state of a specific row and column in a bitboard
bool AUE4_PlaygroundBlockGrid::GetTileState(const int64_t& bitBoard, const int32& row, const int32& column) const
{
	int64_t mask = 1LL << (row * Width + column);

	return ((mask & bitBoard) != 0);
}

ETILETYPE AUE4_PlaygroundBlockGrid::GetTileType(const int32 row, const int32 column)
{
	if ((row == StartRow) && (column == StartCol)) return ETILETYPE::VE_START;

	if ((row == EndRow) && (column == EndCol)) return ETILETYPE::VE_END;

	if (GetTileState(BlockedTilesBitboard, row, column))
	{
		return ETILETYPE::VE_BLOCKED;
	}
	
	if (GetTileState(TurnLeftTilesBitboard, row, column))
	{
		return ETILETYPE::VE_TURN_LEFT;
	}

	else if (GetTileState(StraightTilesBitboard, row, column))
	{
		return ETILETYPE::VE_STRAIGHT;		
	}

	else if (GetTileState(TurnRightTilesBitboard, row, column))
	{
		return ETILETYPE::VE_TURN_RIGHT;		
	}

	return ETILETYPE::VE_BASE;
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



