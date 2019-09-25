// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Definitions.h"
#include "MainCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEndOfMoveDelegate);

UCLASS()
class UE4_PLAYGROUND_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter();

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


protected:
	UPROPERTY(EditDefaultsOnly, Category = "Movement Settings")
	float Speed = 100.0f;


	UPROPERTY(EditDefaultsOnly, Category = "Grid Settings")
	float TileSpacing;
	
private:	

	bool bIsMoving = false;
	float CurrentDistance = 0.0f;
	FVector StartLocation;
	FVector TargetLocation;
	FVector Direction = FVector::ZeroVector;
	float TotalDistance = 0.0f;
	
public:

	FEndOfMoveDelegate OnEndMovement;

	void MoveToDirection(EDIRECTION direction, float distance);

	void RotateToDirection(EDIRECTION direction);
};
