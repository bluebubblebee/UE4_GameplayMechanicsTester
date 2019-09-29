// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGameMenuInterface.h"
#include "InGameUI.generated.h"

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

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:

	// InGameMessage
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* InGameMessage;

	UPROPERTY(meta = (BindWidget))
	class UImage* BackgroundMessage;

	UPROPERTY(meta = (BindWidget))
	class USizeBox* InGameMessageBox;




	

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ContinueMessage;

	UPROPERTY(meta = (BindWidget))
	class UButton* ContinueMessageButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* StartGameButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* RestartGameButton;

protected:

	IInGameMenuInterface* MenuInterface;

protected:

	UFUNCTION()
	void OnStartPathButtonPressed();

	UFUNCTION()
	void OnContinueMessagePressButton();	
	

	UFUNCTION()
	void OnRestartGameButton();

public:

	void SetMenuInteraface(IInGameMenuInterface* Menu);

	// InGameMessage
	void ShowInGameMessage(const FString& Text);

	void HideInGameMessage();






	

	/*

	void DisableContinueMessageButton();

	void ShowContinueMessage();

	void HideContinueMessage();
	*/

public:


	FRestartButtonPressDelegate OnRestartPress;

	FContinueButtonPressDelegate OnContinuePress;


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
	bool bWaitToShowMesageContinue = false;

	float CurrentTimeToShowContinue = 0.0f;

	float DelayToShowContinue = 3.0f;

	
};
