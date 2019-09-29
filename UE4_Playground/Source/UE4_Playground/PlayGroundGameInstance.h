// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "InGameMenuInterface.h"
#include "PlayGroundGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class UE4_PLAYGROUND_API UPlayGroundGameInstance : public UGameInstance, public IInGameMenuInterface
{
	GENERATED_BODY()

protected:
	// Dynamic reference to the blueprint class
	TSubclassOf<class UUserWidget> InGameUIClass;

	// Internal reference to the blueprint for gameplay logic
	UPROPERTY(BlueprintReadWrite, Category = "UI")
	class UInGameUI* InGameUI;

public:	

	UPlayGroundGameInstance(const FObjectInitializer & ObjectInitializer);

	virtual void Init();

	//FORCEINLINE class UInGameUI* GetInGameUI() const { return InGameUI; }

	
public:

	// Interface IInGameMenuInterface

	UFUNCTION(BlueprintCallable)
	void OnLoadInGameMenu() override;

	void OnStartGamePath() override;

	void OnStartGame() override;

	// Interface IInGameMenuInterface

private:

	// Reference to the Grid
	class AUE4_PlaygroundBlockGrid* CurrentGrid;
	
};
