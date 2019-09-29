// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Definitions.h"

#include "UE4_PlaygroundBlockGrid.generated.h"

// Class Grid to manage the path for the character
UCLASS(minimalapi)
class AUE4_PlaygroundBlockGrid : public AActor
{
	GENERATED_BODY()

	/** Dummy root component */
	UPROPERTY(Category = Grid, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* DummyRoot;

public:

	AUE4_PlaygroundBlockGrid();

protected:

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category = "Grid Settings")
    TSubclassOf<class AUE4_PlaygroundBlock> TileClass;

	UPROPERTY(EditDefaultsOnly, Category = "Grid Settings")
	float TileSpacing;

	UPROPERTY(BlueprintReadOnly, Category = "Grid Settings")
	int32 Width;

	UPROPERTY(BlueprintReadOnly, Category = "Grid Settings")
	int32 Height;

	// Start, End Tiles
	UPROPERTY(BlueprintReadOnly, Category = "Grid Settings")
	int32 StartRow;
	UPROPERTY(BlueprintReadOnly, Category = "Grid Settings")
	int32 StartCol;

	UPROPERTY(BlueprintReadOnly, Category = "Grid Settings")
	int32 EndRow;

	UPROPERTY(BlueprintReadOnly, Category = "Grid Settings")
	int32 EndCol;

	UPROPERTY(EditDefaultsOnly, Category = "Grid Settings")
	TSubclassOf<class AMainCharacter> MainCharacterClass;

	UPROPERTY(EditDefaultsOnly, Category = "Grid Settings")
	int MaximunTileClicks;

public:

	void StartGame();

	void OnStartPath();

	void HandleClickedOnTile(class AUE4_PlaygroundBlock* Tile);

	FORCEINLINE class USceneComponent* GetDummyRoot() const { return DummyRoot; }

private:


	bool bIsInputLocked;

	int32 NumberTileClicked;

	class AUE4_PlaygroundBlock* LastBlockClicked = nullptr;

	///// MAIN CHARACTER ///////

	class AMainCharacter* MainCharacter;

	int32 CurrentRowMainChar;

	int32 CurrentColMainChar;	

	void MoveToNextTile();

	bool bWaitForNextMove;

	float waitDeltaTime;

	///// MAIN CHARACTER ///////

	///// MAIN CHARACTER: Path ///////
	ETILETYPE currentTile;

	EDIRECTION currentDirection;
	///// MAIN CHARACTER: Path ///////

private:

	void UpdateClicksText();

	void ShowInGameMessageText(const FString& Text);

private:

	///// BIT BOARD ///////
	// Grid Representation as long long int, 64 bits.
	// Each int64_t represents a tile type
	int64_t BlockedTilesBitboard;
	int64_t StraightTilesBitboard;
	int64_t TurnLeftTilesBitboard;
	int64_t TurnRightTilesBitboard;
	
	int64_t ToggleTile(const int64_t& bitBoard, const int32& row, const int32& column);

	bool GetTileState(const int64_t& bitBoard, const int32& row, const int32& column)  const;

	ETILETYPE GetTileType(const int32 row, const int32 column);

	ETILETYPE GetNextTileType(ETILETYPE currentType);

	///// BIT BOARD ///////
};



