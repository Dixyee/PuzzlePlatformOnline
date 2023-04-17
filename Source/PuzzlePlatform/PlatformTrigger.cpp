// Fill out your copyright notice in the Description page of Project Settings.


#include "PlatformTrigger.h"
#include "Components/BoxComponent.h"

#include "MyOpenDoor.h"
#include "MovingPlatform.h"

// Sets default values
APlatformTrigger::APlatformTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TrigerVolume = CreateDefaultSubobject<UBoxComponent>(FName("TrigerVolume"));
	if (!ensure(TrigerVolume != nullptr)) return;

	RootComponent = TrigerVolume;

	TrigerVolume->OnComponentBeginOverlap.AddDynamic(this, &APlatformTrigger::OnOverLapBegin);
	TrigerVolume->OnComponentEndOverlap.AddDynamic(this, &APlatformTrigger::OnOverLapEnds);
}

// Called when the game starts or when spawned
void APlatformTrigger::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		SetReplicates(true);
		SetReplicateMovement(true);
	}
	TargetLocationPlate += FVector(0.f, 0.f, 95.f);
	GetDefaultLocation = GetActorLocation();
	PlatePressedLocation = GetTransform().TransformPosition(TargetLocationPlate);
}

// Called every frame
void APlatformTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (overlap == true)
	{
		if (HasAuthority())
		{
			FVector Location = GetActorLocation();
		
			FVector Direction = (PlatePressedLocation - Location).GetSafeNormal();
			Location += PresingSpeed * DeltaTime * Direction;
			SetActorLocation(Location);
			
		}
	}
	else if(!overlap)
	{
		if (HasAuthority())
		{
			FVector Location = GetActorLocation();
			
				FVector Direction = (GetDefaultLocation - Location).GetSafeNormal();
				Location += PresingSpeed * DeltaTime * Direction;
				SetActorLocation(Location);
	
		}
	}
}

void APlatformTrigger::OnOverLapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{	
	overlap = true;
	for (AMovingPlatform* Platform : PlatformsToTrigger)
	{
		Platform->AddActiveTrigger();
	}

	for (AMyOpenDoor* Doors : DoorsToTrigger)
	{
		Doors->AddActiveTrigger();
	}
}

void APlatformTrigger::OnOverLapEnds(UPrimitiveComponent* OverlappedComp,AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	overlap = false;
	for (AMovingPlatform * Platform : PlatformsToTrigger)
	{
		Platform->RemoveActiveTrigger();
	}
	for (AMyOpenDoor* Doors : DoorsToTrigger)
	{
		Doors->RemoveActiveTrigger();
	}
}

