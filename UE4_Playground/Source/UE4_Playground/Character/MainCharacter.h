// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Definitions.h"
#include "MainCharacter.generated.h"

UCLASS()
class UE4_PLAYGROUND_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	AMainCharacter();
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Movement Settings")
	float Speed = 100.0f;

	
public:

	void MoveToDirection(EDIRECTION direction, float distance);

	void RotateToDirection(EDIRECTION direction);
};
