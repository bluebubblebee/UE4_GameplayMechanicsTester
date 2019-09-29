// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGameMenuInterface.h"
#include "InGameUI.generated.h"


UCLASS()
class UE4_PLAYGROUND_API UInGameUI : public UUserWidget
{
	GENERATED_BODY()

private:
	virtual bool Initialize() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:

	// InGameMessage
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* InGameMessage;

	UPROPERTY(meta = (BindWidget))
	class UImage* BackgroundMessage;

	UPROPERTY(meta = (BindWidget))
	class USizeBox* InGameMessageBox;	
	
	// Continue Messages
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ContinueMessage;

	UPROPERTY(meta = (BindWidget))
	class UButton* ContinueMessageButton;

	// Game Buttons
	UPROPERTY(meta = (BindWidget))
	class UButton* StartGamePathButton;

	// Game Stats
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ClickCountText;

protected:

	IInGameMenuInterface* MenuInterface;

protected:

	UFUNCTION()
	void OnStartPathButtonPressed();

	UFUNCTION()
	void OnContinueMessagePressButton();	

public:

	void SetMenuInteraface(IInGameMenuInterface* Menu);

	// InGameMessage
	void ShowInGameMessage(const FString& Text, bool ShowContinue = true);

	void HideInGameMessage();

	void UpdateClickCountText(const FString& Text);

private:

	// Typewritter methods
	bool bTypewriterMessage = false;

	FString MessageToTypewrite;

	FString CurrentMessage;

	int32 LetterIndex;

	float CurrentTimeBetweenLetters = 0.0f;

	float DelayBetweenLetters = 0.05f;

	float TotalTimeTypeWritting = 0.0f;

	// Continue Message
	bool bShowContinue = false;

	bool bWaitToShowMesageContinue = false;

	float CurrentTimeToShowContinue = 0.0f;

	float DelayToShowContinue = 1.0f;	
};
