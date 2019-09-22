// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "UE4_PlaygroundPawn.generated.h"

UCLASS(config=Game)
class AUE4_PlaygroundPawn : public APawn
{
	GENERATED_UCLASS_BODY()

public:

	virtual void Tick(float DeltaSeconds) override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual void CalcCamera(float DeltaTime, struct FMinimalViewInfo& OutResult) override;

protected:

	virtual void BeginPlay() override;


	void TriggerClick();
	void TraceForBlock(const FVector& Start, const FVector& End, bool bDrawDebugHelpers);	

	void StartAction();

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	class AUE4_PlaygroundBlock* CurrentBlockFocus;	

	class AUE4_PlaygroundBlockGrid* CurrentGrid;
};
