// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameUI.h"
#include "Components/TextBlock.h"
#include "Components/SizeBox.h"
#include "Components/Image.h"

bool UInGameUI::Initialize()
{
	bool Success = Super::Initialize();

	if (!Success) return false;

	UpdateInGameMessage("");

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