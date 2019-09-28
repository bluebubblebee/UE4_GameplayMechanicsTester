// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGameUI.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStartPathButtonPressDelegate);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRestartButtonPressDelegate);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FContinueButtonPressDelegate);
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
	class UTextBlock* ContinueMessage;

	UPROPERTY(meta = (BindWidget))
	class UButton* ContinueMessageButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* StartGameButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* RestartGameButton;

protected:

	UFUNCTION()
	void OnContinueMessagePressButton();	
	
	UFUNCTION()
	void OnStartPathButtonPressed();

	UFUNCTION()
	void OnRestartGameButton();

public:

	void ShowMessages();

	void HideMessages();

	void UpdateInGameMessage(const FString& Text);


	void DisableContinueMessageButton();

	void ShowContinueMessage();

	void HideContinueMessage();

public:

	FStartPathButtonPressDelegate OnStartPathPress;

	FRestartButtonPressDelegate OnRestartPress;

	FContinueButtonPressDelegate OnContinuePress;
	
};
