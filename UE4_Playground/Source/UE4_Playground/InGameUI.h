// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGameUI.generated.h"

/**
 * 
 */
UCLASS()
class UE4_PLAYGROUND_API UInGameUI : public UUserWidget
{
	GENERATED_BODY()

private:
	virtual bool Initialize() override;

protected:

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* InGameMessages;

public:

	void UpdateInGameMessage(const FString& Text);
	
};
