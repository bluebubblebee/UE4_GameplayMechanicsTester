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

	UpdateInGameMessage("Click on the tiles to change its type.\nPress the Play Button to make Taichi follow the path");

	HideContinueMessage();

	return true;
}

void UInGameUI::ShowMessages()
{
	if (InGameBox != nullptr)
	{
		InGameBox->SetVisibility(ESlateVisibility::Visible);
	}

	if (BackgroundMessages != nullptr)
	{
		BackgroundMessages->SetVisibility(ESlateVisibility::Visible);
	}
}

void UInGameUI::HideMessages()
{
	if(InGameBox != nullptr)
	{
		InGameBox->SetVisibility(ESlateVisibility::Hidden);
	}

	if (BackgroundMessages != nullptr)
	{
		BackgroundMessages->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UInGameUI::UpdateInGameMessage(const FString& Text)
{
	if (InGameMessages == nullptr) return;

	InGameMessages->SetText(FText::FromString(Text)); 
}

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


void UInGameUI::OnStartPathButtonPressed()
{
	OnStartPathPress.Broadcast();
}


void UInGameUI::OnRestartGameButton()
{
	OnRestartPress.Broadcast();
}

void UInGameUI::OnContinueMessagePressButton()
{
	OnContinuePress.Broadcast();
}