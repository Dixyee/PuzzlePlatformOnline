// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzlePlatformsGameInstance.h"

#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"

#include <PuzzlePlatform/MainMenu.h>
#include <PuzzlePlatform/MenuWidget.h>



UPuzzlePlatformsGameInstance::UPuzzlePlatformsGameInstance(const FObjectInitializer & ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> MenuBPClass(TEXT("/Game/ThirdPerson/MenuSystem/WBP_MainMenu")); //Map where the Widget will be created
	if (!ensure(MenuBPClass.Class != NULL)) return;
	
	MenuClass = MenuBPClass.Class;

	ConstructorHelpers::FClassFinder<UUserWidget> InGameMenu(TEXT("/Game/ThirdPerson/MenuSystem/WBP_Overlay"));
	if (!ensure(InGameMenu.Class != NULL)) return;

	InGameMenuClass = InGameMenu.Class;
}
void UPuzzlePlatformsGameInstance::Init()
{
	UE_LOG(LogTemp, Warning, TEXT("Found class %s"), *MenuClass->GetName());

}

void UPuzzlePlatformsGameInstance::LoadMenu()
{
	if (!ensure(MenuClass != NULL)) return;
	Menu = CreateWidget<UMainMenu>(this, MenuClass);
	if (!ensure(Menu != NULL)) return;
	
	Menu->Setup();
	Menu->SetMenuInterface(this);
}

void UPuzzlePlatformsGameInstance::InGameLoadMenu()
{
	if (!ensure(InGameMenuClass != NULL)) return;
	UMenuWidget* Overlay = CreateWidget<UMenuWidget>(this, InGameMenuClass);
	if (!ensure(Overlay != NULL)) return;

	Overlay->Setup();
	Overlay->SetMenuInterface(this);
}

void UPuzzlePlatformsGameInstance::Host()
{
	if (Menu != nullptr)
	{
		Menu->Teardown();
	}

	UEngine* Engine = GetEngine();
	if (!ensure(Engine != NULL)) return;
	
	Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Hosting"));

	UWorld* World = GetWorld();
		if (!ensure(World != NULL)) return;

	World->ServerTravel("/Game/ThirdPerson/Maps/MP_PP_Level1?listen");

	
}

void UPuzzlePlatformsGameInstance::PortNumber()
{
	UEngine* Engine = GetEngine();
	if (!ensure(Engine != NULL)) return;
	
	UWorld* World = GetWorld();
	if (!ensure(World != NULL)) return;
	FString PortNumber = FString::FromInt(GetWorld()->URL.Port);
	Engine->AddOnScreenDebugMessage(0, 10, FColor::Green, FString::Printf(TEXT("Server Port : %s"), *PortNumber));
}

void UPuzzlePlatformsGameInstance::Join(const FString& Adress)
{
	if (Menu != nullptr)
	{
		Menu->Teardown();
	}
	UEngine* Engine = GetEngine();
	if (!ensure(Engine != NULL)) return;

	Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, FString::Printf(TEXT("Joining %s"), *Adress));

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != NULL)) return;

	PlayerController->ClientTravel(*Adress, ETravelType::TRAVEL_Absolute);
}
void UPuzzlePlatformsGameInstance::LoadMainMenu()
{

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != NULL)) return;

	PlayerController->ClientTravel("/Game/ThirdPerson/Maps/MP_MainMenu", ETravelType::TRAVEL_Absolute);
}

