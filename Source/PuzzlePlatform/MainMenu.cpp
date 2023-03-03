// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "MenuInterface.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"

bool UMainMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	//TODO:Setup
	if (!ensure(HostButton != nullptr)) return false;
	HostButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);

	if (!ensure(JoinButton != nullptr)) return false;
	JoinButton->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);
	
	if (!ensure(JoinServerButton != nullptr)) return false;
	JoinServerButton->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);
	
	if (!ensure(CancelButton != nullptr)) return false;
	CancelButton->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu);
	
	return true;
}

void UMainMenu::HostServer()
{
	
	if (MenuInterface != nullptr)
	{
		MenuInterface->Host();
		UE_LOG(LogTemp, Warning, TEXT("TEST"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("NULL POINTER"));

	}
}
void UMainMenu::JoinServer()
{
	if (MenuInterface != nullptr)
	{	
		if (!ensure(ServerTextBox != nullptr)) return;
		FString Adress = ServerTextBox->GetText().ToString();
		MenuInterface->Join(Adress);
		UE_LOG(LogTemp, Warning, TEXT("Succes"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("FAIL"));
	}
	
}
void UMainMenu::OpenJoinMenu()
{
	if (!ensure(CancelButton != nullptr)) return;
	if (!ensure(JoinMenu != nullptr)) return;

	MenuSwitcher->SetActiveWidget(JoinMenu);
}
void UMainMenu::OpenMainMenu()
{
	if (!ensure(MenuSwitcher != nullptr)) return;
	if (!ensure(MainMenu != nullptr)) return;

	MenuSwitcher->SetActiveWidget(MainMenu);
}
