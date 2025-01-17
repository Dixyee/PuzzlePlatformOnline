// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlatformTrigger.generated.h"

UCLASS()
class PUZZLEPLATFORM_API APlatformTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlatformTrigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	bool overlap = false;

	UPROPERTY(EditAnywhere)
		float PresingSpeed = 20;

	UPROPERTY(EditAnywhere , Meta = (MakeEditWidget = true))
		FVector TargetLocationPlate = FVector(0.f,0.f,-95.f);
	

private:
	FVector GetDefaultLocation;
	FVector PlatePressedLocation;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* TrigerVolume;

	UPROPERTY(EditAnywhere)
		TArray<class AMovingPlatform*> PlatformsToTrigger;

	UFUNCTION()
		void OnOverLapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult );
	UFUNCTION()
		void OnOverLapEnds(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
