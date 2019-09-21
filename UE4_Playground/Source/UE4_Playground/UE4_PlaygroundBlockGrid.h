// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UE4_PlaygroundBlockGrid.generated.h"

/** Class used to spawn blocks and manage score */
UCLASS(minimalapi)
class AUE4_PlaygroundBlockGrid : public AActor
{
	GENERATED_BODY()

	/** Dummy root component */
	UPROPERTY(Category = Grid, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* DummyRoot;

	/** Text component for the score */
	UPROPERTY(Category = Grid, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UTextRenderComponent* ScoreText;

public:

	AUE4_PlaygroundBlockGrid();

protected:


	// Begin AActor interface
	virtual void BeginPlay() override;
	// End AActor interface

	UPROPERTY(EditDefaultsOnly, Category = "Grid Settings")
    TSubclassOf<class AUE4_PlaygroundBlock> BlockClass;

	UPROPERTY(EditDefaultsOnly, Category = "Grid Settings")
	TSubclassOf<class AMainCharacter> MainCharacterClass;

	UPROPERTY(EditDefaultsOnly, Category = "Grid Settings")
	int MaximunBlockClicks;

	UPROPERTY(BlueprintReadOnly, Category = "Grid Settings")
	int32 Width;

	UPROPERTY(BlueprintReadOnly, Category = "Grid Settings")
	int32 Height;

	UPROPERTY(EditDefaultsOnly, Category = "Grid Settings")
	float BlockSpacing;


public:

	/** Handle the block being clicked */
	void AddScore();

	void SetBlockClicked(class AUE4_PlaygroundBlock* Block);

	FORCEINLINE class USceneComponent* GetDummyRoot() const { return DummyRoot; }

	FORCEINLINE class UTextRenderComponent* GetScoreText() const { return ScoreText; }


private:

	int32 Score;

	int32 NumberBlocksClicked;

	class AUE4_PlaygroundBlock* LastBlockClicked = nullptr;

	class AMainCharacter* MainCharacter;

private:
	///// BIT BOARD ///////

	// Grid Representation, long is 64 bits
	long GridBitboard;

	// Bit board represeting the blocks
	long BlocksBitboard;

	long SetTileState(const long& bitBoard, const int32& row, const int32& column);

	bool GetTileState(const long& bitBoard, const int32& row, const int32& column)  const;

	///// BIT BOARD ///////

};



