// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "UE4_PlaygroundPlayerController.generated.h"

/** PlayerController class used to enable cursor */
UCLASS()
class AUE4_PlaygroundPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AUE4_PlaygroundPlayerController();


protected:
	// Dynamic reference to the blueprint class
	TSubclassOf<class UUserWidget> InGameUIClass;

	// Internal reference to the blueprint for gameplay logic
	UPROPERTY(BlueprintReadWrite, Category = "UI")
	class UInGameUI* InGameUI;

public:
	FORCEINLINE class UInGameUI* GetInGameUI() const { return InGameUI; }

protected:

	virtual void BeginPlay() override;
};


