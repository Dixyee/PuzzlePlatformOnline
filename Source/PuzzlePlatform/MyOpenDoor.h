// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "MyOpenDoor.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORM_API AMyOpenDoor : public AStaticMeshActor
{
	GENERATED_BODY()
	
public:
	AMyOpenDoor();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
		float speed = 20;

	void AddActiveTrigger();
	void RemoveActiveTrigger();

	UFUNCTION(BlueprintNativeEvent)
	void OpenDoor();

	UFUNCTION(BlueprintNativeEvent)
		void CloseDoor();

private:

	UPROPERTY(EditAnywhere)
		int ActiveTrigger = 1;

};
