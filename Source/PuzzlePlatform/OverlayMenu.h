// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "OverlayMenu.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORM_API UOverlayMenu : public UMenuWidget
{
	GENERATED_BODY()

protected:
	virtual bool Initialize();

private:
	UPROPERTY(meta = (BindWidget))
		class UButton* QuitButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* CancelButton;
	
	UFUNCTION()
		void QuitServer();

	UFUNCTION()
		void ExistWidget(); // Cancel the overlay
};
