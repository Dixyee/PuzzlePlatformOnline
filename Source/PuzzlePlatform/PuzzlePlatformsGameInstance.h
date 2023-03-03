// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MenuInterface.h"

#include "PuzzlePlatformsGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORM_API UPuzzlePlatformsGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()
public:
	UPuzzlePlatformsGameInstance(const FObjectInitializer & ObjectInitializer);
	virtual void Init();

	UFUNCTION(BlueprintCallable)
		void LoadMenu();

	UFUNCTION(BlueprintCallable)
		void InGameLoadMenu();

	UFUNCTION(Exec)
		void Host() override;

	UFUNCTION(Exec)
		void Join(const FString& Adress) override;

	UFUNCTION(Exec)
		void PortNumber();

	virtual void LoadMainMenu() override;
private:
	TSubclassOf<class UUserWidget> MenuClass;
	TSubclassOf<class UUserWidget> InGameMenuClass;
	

	class UMainMenu* Menu;


};
