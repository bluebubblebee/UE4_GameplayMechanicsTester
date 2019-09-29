// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InGameMenuInterface.generated.h"

UINTERFACE(MinimalAPI)
class UInGameMenuInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UE4_PLAYGROUND_API IInGameMenuInterface
{
	GENERATED_BODY()

public:
	

	virtual void OnLoadInGameMenu() = 0;

	virtual void OnStartGamePath() = 0;

	virtual void OnStartGame() = 0;
};
