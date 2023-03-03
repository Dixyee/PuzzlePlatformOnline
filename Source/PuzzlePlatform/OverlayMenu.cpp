// Fill out your copyright notice in the Description page of Project Settings.


#include "OverlayMenu.h"
#include "PuzzlePlatformsGameInstance.h"
#include "Components/Button.h"

bool UOverlayMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;


	if (!ensure(QuitButton != nullptr)) return false;
	QuitButton->OnClicked.AddDynamic(this, &UOverlayMenu::QuitServer);

	if (!ensure(CancelButton != nullptr)) return false;
	CancelButton->OnClicked.AddDynamic(this, &UOverlayMenu::ExistWidget);

	return true;
}

void UOverlayMenu::QuitServer()
{
	if (MenuInterface != nullptr)
	{
		Teardown();
		MenuInterface->LoadMainMenu();
	}
}
void UOverlayMenu::ExistWidget()
{
	
	Teardown();
}