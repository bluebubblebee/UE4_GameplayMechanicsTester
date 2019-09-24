// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Definitions.h"
#include "UE4_PlaygroundBlock.generated.h"

// Class Tile clicklable
UCLASS()
class AUE4_PlaygroundBlock : public AActor
{
	GENERATED_BODY()

	/** Dummy root component */
	UPROPERTY(Category = Block, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* DummyRoot;

	/** StaticMesh component for the clickable block */
	UPROPERTY(Category = Block, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* BlockMesh;

public:
	AUE4_PlaygroundBlock();

	/** Are we currently active? */
	bool bIsActive;

	/** Pointer to white material used on the focused block */
	UPROPERTY()
	class UMaterial* BaseMaterial;

	// Pointer to each material instace for the different tiles
	UPROPERTY()
	class UMaterialInstance* BlockTileMaterial;

	UPROPERTY()
	class UMaterialInstance* TurnLeftTileMaterial;

	UPROPERTY()
	class UMaterialInstance* TurnRightTileMaterial;	

	UPROPERTY()
	class UMaterialInstance* StraightTileMaterial;

	UPROPERTY()
	class UMaterialInstance* StartTileMaterial;

	UPROPERTY()
	class UMaterialInstance* ExitTileMaterial;


	/** Grid that owns us */
	UPROPERTY()
	class AUE4_PlaygroundBlockGrid* OwningGrid;

	/** Handle the block being clicked */
	UFUNCTION()
	void BlockClicked(UPrimitiveComponent* ClickedComp, FKey ButtonClicked);

	/** Handle the block being touched  */
	UFUNCTION()
	void OnFingerPressedBlock(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent);

	void HandleClicked();

public:
	/** Returns DummyRoot subobject **/
	FORCEINLINE class USceneComponent* GetDummyRoot() const { return DummyRoot; }
	/** Returns BlockMesh subobject **/
	FORCEINLINE class UStaticMeshComponent* GetBlockMesh() const { return BlockMesh; }

	FORCEINLINE int32 GetRow() const { return RowInGrid; }

	FORCEINLINE void SetRow(int32 row) { RowInGrid = row; }

	FORCEINLINE int32 GetCol() const { return ColInGrid; }

	FORCEINLINE void SetCol(int32 col) { ColInGrid = col; }

	void SetType(ETILETYPE type);

private:

	int32 RowInGrid;
	int32 ColInGrid;

};



