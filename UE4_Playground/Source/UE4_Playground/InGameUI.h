// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGameUI.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEndOfStartPressDelegate);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEndRestartPressDelegate);
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

	UPROPERTY(meta = (BindWidget))
	class UImage* BackgroundMessages;

	UPROPERTY(meta = (BindWidget))
	class USizeBox* InGameBox;


	UPROPERTY(meta = (BindWidget))
	class UButton* StartGameButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* RestartGameButton;

protected:

	UFUNCTION()
	void OnStartGamePressed();

	UFUNCTION()
	void OnRestartGameButton();

public:

	void ShowMessages();

	void HideMessages();

	void UpdateInGameMessage(const FString& Text);

public:

	FEndOfStartPressDelegate OnStartPress;

	FEndRestartPressDelegate OnRestartPress;
	
};
