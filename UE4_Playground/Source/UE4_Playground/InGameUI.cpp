// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameUI.h"
#include "Components/TextBlock.h"
#include "Components/SizeBox.h"
#include "Components/Image.h"
#include "Components/Button.h"

bool UInGameUI::Initialize()
{
	bool Success = Super::Initialize();

	if (!Success) return false;

	if (StartGameButton == nullptr) return false;
	StartGameButton->OnClicked.AddDynamic(this, &UInGameUI::OnStartPathButtonPressed);

	if (RestartGameButton == nullptr) return false;
	RestartGameButton->OnClicked.AddDynamic(this, &UInGameUI::OnRestartGameButton);

	if (ContinueMessageButton == nullptr) return false;
	ContinueMessageButton->OnClicked.AddDynamic(this, &UInGameUI::OnContinueMessagePressButton);


	if (ContinueMessageButton != nullptr)
	{
		ContinueMessageButton->SetVisibility(ESlateVisibility::Hidden);
	}

	if (ContinueMessage != nullptr)
	{
		ContinueMessage->SetVisibility(ESlateVisibility::Hidden);
	}

	return true;
}

void UInGameUI::SetMenuInteraface(IInGameMenuInterface* Menu)
{
	MenuInterface = Menu;
}


void UInGameUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::Tick(MyGeometry, InDeltaTime);

	// Tyepwriter message
	if (bTypewriterMessage)
	{
		CurrentTimeBetweenLetters += InDeltaTime;

		if (CurrentTimeBetweenLetters >= DelayBetweenLetters)
		{
			CurrentTimeBetweenLetters = 0.0f;

			CurrentMessage += MessageToTypewrite[LetterIndex];

			InGameMessage->SetText(FText::FromString(CurrentMessage));

			LetterIndex += 1;

			if (LetterIndex >= MessageToTypewrite.Len())
			{
				CurrentTimeBetweenLetters = 0.0f;
				bTypewriterMessage = false;

				bWaitToShowMesageContinue = true;
				CurrentTimeToShowContinue = 0.0f;

				
			}
		}
	}

	// Continue Message
	if (bWaitToShowMesageContinue)
	{
		CurrentTimeToShowContinue += InDeltaTime;

		if (CurrentTimeToShowContinue >= DelayToShowContinue)
		{
			if (ContinueMessageButton != nullptr)
			{
				ContinueMessageButton->SetVisibility(ESlateVisibility::Visible);
			}

			if (ContinueMessage != nullptr)
			{
				ContinueMessage->SetVisibility(ESlateVisibility::Visible);
			}

			bWaitToShowMesageContinue = false;
		}
	}
}

void UInGameUI::ShowInGameMessage(const FString& Text)
{
	bWaitToShowMesageContinue = false;
	bTypewriterMessage = false;

	if (InGameMessage == nullptr) return;

	InGameMessage->SetText(FText::FromString(""));

	if (InGameMessageBox != nullptr)
	{
		InGameMessageBox->SetVisibility(ESlateVisibility::Visible);
	}

	if (BackgroundMessage != nullptr)
	{
		BackgroundMessage->SetVisibility(ESlateVisibility::Visible);
	}
	

	LetterIndex = 0;

	CurrentMessage = "";

	MessageToTypewrite = Text;

	TotalTimeTypeWritting = DelayBetweenLetters * MessageToTypewrite.Len();

	CurrentTimeBetweenLetters = 0.0f;

	bTypewriterMessage = true;
}


void UInGameUI::HideInGameMessage()
{
	if(InGameMessageBox != nullptr)
	{
		InGameMessageBox->SetVisibility(ESlateVisibility::Hidden);
	}

	if (BackgroundMessage != nullptr)
	{
		BackgroundMessage->SetVisibility(ESlateVisibility::Hidden);
	}
}











/*
void UInGameUI::DisableContinueMessageButton()
{
	if (ContinueMessageButton != nullptr)
	{
		ContinueMessageButton->SetVisibility(ESlateVisibility::Hidden);
	}
}


void UInGameUI::ShowContinueMessage()
{	
	if (ContinueMessage != nullptr)
	{
		ContinueMessage->SetVisibility(ESlateVisibility::Visible);
	}
}

void UInGameUI::HideContinueMessage()
{
	if (ContinueMessage != nullptr)
	{
		ContinueMessage->SetVisibility(ESlateVisibility::Hidden);
	}
}
*/


void UInGameUI::OnStartPathButtonPressed()
{
	if (MenuInterface == nullptr) return;

	MenuInterface->OnStartGamePath();
}


void UInGameUI::OnRestartGameButton()
{
	//OnRestartPress.Broadcast();
}

void UInGameUI::OnContinueMessagePressButton()
{
	UE_LOG(LogTemp, Warning, TEXT("[UInGameUI::OnContinueMessagePressButton] "));

	// Hide all
	if (ContinueMessageButton != nullptr)
	{
		ContinueMessageButton->SetVisibility(ESlateVisibility::Hidden);
	}

	if (ContinueMessage != nullptr)
	{
		ContinueMessage->SetVisibility(ESlateVisibility::Hidden);
	}


	if (InGameMessageBox != nullptr)
	{
		InGameMessageBox->SetVisibility(ESlateVisibility::Hidden);
	}

	if (BackgroundMessage != nullptr)
	{
		BackgroundMessage->SetVisibility(ESlateVisibility::Hidden);
	}

	if (MenuInterface != nullptr)
	{
		MenuInterface->OnStartGame();
	}

	//OnContinuePress.Broadcast();
}