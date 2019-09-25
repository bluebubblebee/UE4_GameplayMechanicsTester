// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "UE4_PlaygroundBlock.h"
#include "UE4_PlaygroundBlockGrid.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInstance.h"

AUE4_PlaygroundBlock::AUE4_PlaygroundBlock()
{
	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> PlaneMesh;
		ConstructorHelpers::FObjectFinderOptional<UMaterial> BaseMaterial;

		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> MTurnLeft;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> MTurnRight;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> MStraight;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> MBlock;   
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> MStart;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> MExit;
		

		FConstructorStatics()
			: PlaneMesh(TEXT("/Game/Art/Tiles/Tile.Tile"))
			, BaseMaterial(TEXT("/Game/Art/Tiles/M_BaseMaterial.M_BaseMaterial"))

			, MTurnLeft(TEXT("/Game/Art/Tiles/MI_TurnLeft.MI_TurnLeft"))
			, MTurnRight(TEXT("/Game/Art/Tiles/MI_TurnRight.MI_TurnRight"))
			, MStraight(TEXT("/Game/Art/Tiles/MI_Straight.MI_Straight"))
			, MBlock(TEXT("/Game/Art/Tiles/MI_Block.MI_Block"))
			, MStart(TEXT("/Game/Art/Tiles/MI_Start.MI_Start"))
			, MExit(TEXT("/Game/Art/Tiles/MI_Exit.MI_Exit"))
			

		{
		}
	};


	static FConstructorStatics ConstructorStatics;

	// Create dummy root scene component
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = DummyRoot;

	// Create static mesh component
	BlockMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlockMesh0"));
	BlockMesh->SetStaticMesh(ConstructorStatics.PlaneMesh.Get());
	BlockMesh->SetRelativeScale3D(FVector(1.f,1.f,0.25f));
	BlockMesh->SetRelativeLocation(FVector(0.f,0.f,25.f));
	BlockMesh->SetMaterial(0, ConstructorStatics.BaseMaterial.Get());
	BlockMesh->SetupAttachment(DummyRoot);
	BlockMesh->OnClicked.AddDynamic(this, &AUE4_PlaygroundBlock::BlockClicked);
	BlockMesh->OnInputTouchBegin.AddDynamic(this, &AUE4_PlaygroundBlock::OnFingerPressedBlock);

	// Save a pointer to the orange material
	BaseMaterial = ConstructorStatics.BaseMaterial.Get();

	TurnLeftTileMaterial = ConstructorStatics.MTurnLeft.Get();
	TurnRightTileMaterial = ConstructorStatics.MTurnRight.Get();
	BlockTileMaterial = ConstructorStatics.MBlock.Get();
	StraightTileMaterial = ConstructorStatics.MStraight.Get();
	StartTileMaterial = ConstructorStatics.MStart.Get();
	ExitTileMaterial = ConstructorStatics.MExit.Get();

}

void AUE4_PlaygroundBlock::BlockClicked(UPrimitiveComponent* ClickedComp, FKey ButtonClicked)
{
	HandleClicked();
}


void AUE4_PlaygroundBlock::OnFingerPressedBlock(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent)
{
	HandleClicked();
}

void AUE4_PlaygroundBlock::HandleClicked()
{

	if (OwningGrid != nullptr)
	{
		OwningGrid->HandleClickedOnTile(this);
	}
}


void AUE4_PlaygroundBlock::SetType(ETILETYPE type)
{
	switch (type)
	{
	case ETILETYPE::VE_BLOCKED:
		BlockMesh->SetMaterial(0, BlockTileMaterial);
		break;
	
	case ETILETYPE::VE_TURN_LEFT:

		BlockMesh->SetMaterial(0, TurnLeftTileMaterial);
		break;
	case ETILETYPE::VE_TURN_RIGHT:

		BlockMesh->SetMaterial(0, TurnRightTileMaterial);
		break;
	case ETILETYPE::VE_STRAIGHT:
		BlockMesh->SetMaterial(0, StraightTileMaterial);
		break;
	case ETILETYPE::VE_START:
		BlockMesh->SetMaterial(0, StartTileMaterial);
		break;
	case ETILETYPE::VE_END:
		BlockMesh->SetMaterial(0, ExitTileMaterial);
		break;
	}
}


