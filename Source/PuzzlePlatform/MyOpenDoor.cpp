// Fill out your copyright notice in the Description page of Project Settings.


#include "MyOpenDoor.h"

AMyOpenDoor::AMyOpenDoor()
{
	PrimaryActorTick.bCanEverTick = true;

	SetMobility(EComponentMobility::Movable);

}
void AMyOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		SetReplicates(true);
		SetReplicateMovement(true);
	}
}
void AMyOpenDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ActiveTrigger > 0)
	{
		if (HasAuthority())
		{
			OpenDoor();
		}
	}
	if (ActiveTrigger <= 0)
	{
		if (HasAuthority())
		{
			CloseDoor();
		}
	}
}

void AMyOpenDoor::AddActiveTrigger()
{
	ActiveTrigger++;
}
void AMyOpenDoor::RemoveActiveTrigger()
{
	if (ActiveTrigger > 0)
	{
		ActiveTrigger--;
	}
}
void AMyOpenDoor::OpenDoor_Implementation()
{

}
void AMyOpenDoor::CloseDoor_Implementation()
{

}