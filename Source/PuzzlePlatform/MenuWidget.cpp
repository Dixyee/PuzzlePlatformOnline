// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuWidget.h"

void UMenuWidget::Setup()
{
	this->AddToViewport();
	this->bIsFocusable = true;

	UWorld* World = GetWorld();
	if (!ensure(World != NULL)) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != NULL)) return;

	FInputModeUIOnly InputModeData;
	InputModeData.SetWidgetToFocus(this->TakeWidget());
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	PlayerController->SetInputMode(InputModeData);

	PlayerController->bShowMouseCursor = true;


}
void UMenuWidget::Teardown()
{
	this->RemoveFromViewport();
	this->bIsFocusable = false;

	UWorld* World = GetWorld();
	if (!ensure(World != NULL)) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != NULL)) return;

	FInputModeGameOnly InputModeData;

	PlayerController->SetInputMode(InputModeData);

	PlayerController->bShowMouseCursor = false;
}
void UMenuWidget::SetMenuInterface(IMenuInterface* menuInterface)
{
	this->MenuInterface = menuInterface;
}