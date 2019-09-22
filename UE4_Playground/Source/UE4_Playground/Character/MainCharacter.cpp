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

