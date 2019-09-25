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
		

		//UE_LOG(LogTemp, Warning, TEXT(" Walk:  %f / %f "), CurrentDistance, TotalDistance);

		if (CurrentDistance < TotalDistance)
		{
			FVector NewLocation = GetActorLocation();

			NewLocation += Direction * Speed * DeltaTime;

			SetActorLocation(NewLocation);			
			
			//bIsMoving = false;

		}
		else
		{
			//bIsReachTargetLocation = true;

			
			// Sent event
			CurrentDistance = 0.0f;
			bIsMoving = false;
			SetActorLocation(TargetLocation);

			OnEndMovement.Broadcast();

			

			

			//FRotator newRotation = GetActorRotation() + FRotator(0.0f, 90.0f, 0.0f);
			//SetActorRotation(newRotation);
		}

		CurrentDistance = FVector::Distance(GetActorLocation(), StartLocation);

	}
}



// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}



void AMainCharacter::MoveToDirection(EDIRECTION direction, float distance)
{
	if (bIsMoving) return;
		 
	CurrentDistance = 0.0f;

	StartLocation = GetActorLocation();

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

	TargetLocation = NewLocation;

	TotalDistance = FVector::Distance(StartLocation, TargetLocation);	

	Direction = TargetLocation - StartLocation;
	Direction.Normalize();

	UE_LOG(LogTemp, Warning, TEXT("About to move: %f / %f"), CurrentDistance, TotalDistance);

	bIsMoving = true;
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

