// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatform.h"

AMovingPlatform::AMovingPlatform()
{
	PrimaryActorTick.bCanEverTick = true;

	SetMobility(EComponentMobility::Movable);

}
void AMovingPlatform::BeginPlay() 
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		SetReplicates(true);
		SetReplicateMovement(true);
	}

	GlobalStartLocation = GetActorLocation();//Point A
	GlobalTargetLocation = GetTransform().TransformPosition(TargetLocation);//Point B
}
void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (ActiveTrigger > 0)
	{
		if (HasAuthority())
		{

			FVector Location = GetActorLocation(); // Point C 
			float JourneyLenght = (GlobalTargetLocation - GlobalStartLocation).Size(); //The lenght From point B to A
			float JourneyTravelled = (Location - GlobalStartLocation).Size(); //The lengt from point C to A

			// if the lenght of C to A is bigger than B to A	
			//		then swap B and A

			if (JourneyTravelled >= JourneyLenght)
			{
				FVector Swap = GlobalStartLocation;
				GlobalStartLocation = GlobalTargetLocation;
				GlobalTargetLocation = Swap;
			}

			FVector Direction = (GlobalTargetLocation - GlobalStartLocation).GetSafeNormal();
			Location += speed * DeltaTime * Direction;
			SetActorLocation(Location);

		}
	}

}
void AMovingPlatform::AddActiveTrigger()
{
	ActiveTrigger++;
}
void AMovingPlatform::RemoveActiveTrigger()
{
	if (ActiveTrigger > 0)
	{
		ActiveTrigger--;
	}
}