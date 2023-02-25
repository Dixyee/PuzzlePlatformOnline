// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PuzzlePlatformsGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORM_API UPuzzlePlatformsGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UPuzzlePlatformsGameInstance(const FObjecttInitializer& ObjectInitializer);
	virtual void Init();
};
