// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameUI.h"
#include "Components/TextBlock.h"

bool UInGameUI::Initialize()
{
	bool Success = Super::Initialize();

	if (!Success) return false;

	UpdateInGameMessage("");

	return true;
}

void UInGameUI::UpdateInGameMessage(const FString& Text)
{
	if (InGameMessages == nullptr) return;

	InGameMessages->SetText(FText::FromString(Text));
}