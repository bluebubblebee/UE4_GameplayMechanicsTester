// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMainCharacter::MoveToDirection(EDIRECTION direction, float distance)
{

	FVector NewLocation = GetActorLocation();

	switch (direction)
	{
	case EDIRECTION::VE_UP:
		NewLocation.X += distance;
		break;
	case EDIRECTION::VE_DOWN:
		NewLocation.X -= distance;
		break;
	case EDIRECTION::VE_LEFT:
		NewLocation.Y -= distance;
		break;
	case EDIRECTION::VE_RIGHT:
		NewLocation.Y += distance;
		break;
	}
	SetActorLocation(NewLocation);
}

void AMainCharacter::RotateToDirection(EDIRECTION direction)
{
	FRotator AddedRotation = FRotator(0.0f, 0.0f, 0.0f);;

	if (direction == EDIRECTION::VE_RIGHT)
	{
		AddedRotation = FRotator(0.0f, 90.0f, 0.0f);
	}
	else if (direction == EDIRECTION::VE_LEFT)
	{
		AddedRotation = FRotator(0.0f, -90.0f, 0.0f);
	}
	
	SetActorRotation(GetActorRotation() + AddedRotation);
}

