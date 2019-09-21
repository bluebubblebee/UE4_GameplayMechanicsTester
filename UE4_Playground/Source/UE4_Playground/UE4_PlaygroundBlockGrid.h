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


	

	/** Number of blocks along each side of grid */
	//UPROPERTY(Category=Grid, EditAnywhere, BlueprintReadOnly)
	//int32 Size;

	/** Spacing of blocks */
	//UPROPERTY(Category=Grid, EditAnywhere, BlueprintReadOnly)
	//float BlockSpacing;


protected:


	// Begin AActor interface
	virtual void BeginPlay() override;
	// End AActor interface

	UPROPERTY(EditDefaultsOnly, Category = "Grid Settings")
    TSubclassOf<class AUE4_PlaygroundBlock> BlockClass;

	UPROPERTY(EditDefaultsOnly, Category = "Grid Settings")
	int MaximunBlockClicks;

	/** Number of blocks along each side of grid */
	UPROPERTY(EditDefaultsOnly, Category = "Grid Settings")
	int32 Size;


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

	class AUE4_PlaygroundBlock* lastBlockClicked = nullptr;



};



