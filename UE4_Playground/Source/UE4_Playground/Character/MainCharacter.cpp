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

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsMoving)
	{
		CurrentDistance = FVector::Distance(GetActorLocation(), StartLocation);

		if (CurrentDistance < TotalDistance)
		{

			FVector NewLocation = GetActorLocation();

			NewLocation += Direction * Speed * DeltaTime;

			SetActorLocation(NewLocation);

		}
		else
		{
			//bIsReachTargetLocation = true;

			bIsMoving = false;

			SetActorLocation(TargetLocation);

			// Launch OnEndMovement event
			OnEndMovement.Broadcast();

			FRotator newRotation = GetActorRotation() + FRotator(0.0f, 90.0f, 0.0f);
			SetActorRotation(newRotation);
		}

	}
}



// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMainCharacter::WalkPath(TArray<ETILETYPE> TileListToWalk)
{
	TileList = TileListToWalk;
	CurrentTile = 0;


}


void AMainCharacter::DecideNextTile()
{
	if (CurrentTile < TileList.Num())
	{
		switch (TileList[CurrentTile])
		{
			case ETILETYPE::VE_STRAIGHT:

				// Set target position

			break;
			case ETILETYPE::VE_TURN_LEFT:

				break;
			case ETILETYPE::VE_TURN_RIGHT:

				break;
			case ETILETYPE::VE_END:

				// TODO: VICTORY ANIMATION

			break;
		}
	}


	// TODO: IF NOT END TILE FOUND, DEFEAT ANIMATION
}



void AMainCharacter::MoveToPosition(FVector position)
{
	TargetLocation = position;
	StartLocation = GetActorLocation();
	TotalDistance = FVector::Distance(StartLocation, TargetLocation);

	Direction = TargetLocation - StartLocation;
	Direction.Normalize();

	CurrentDistance = 0.0f;

	bIsMoving = true;
}


void AMainCharacter::MoveToDirection(EDIRECTION direction, float distance)
{
	FVector CurrentLocation = GetActorLocation();

	//yPostion, xPostion

	switch (direction)
	{
		case EDIRECTION::VE_UP:
			CurrentLocation.X += distance;
		break;
		case EDIRECTION::VE_DOWN:
			CurrentLocation.X -= distance;
			break;
		case EDIRECTION::VE_LEFT:
			CurrentLocation.Y -= distance;
		break;
		case EDIRECTION::VE_RIGHT:
			CurrentLocation.Y += distance;
		break;
	}

	SetActorLocation(CurrentLocation);
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

