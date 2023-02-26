// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzlePlatformsGameInstance.h"

#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "PlatformTrigger.h"
UPuzzlePlatformsGameInstance::UPuzzlePlatformsGameInstance(const FObjectInitializer & ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> MenuBPClass(TEXT("/Game/ThirdPerson/MenuSystem/WBP_MainMenu"));
	if (!ensure(MenuBPClass.Class != NULL)) return;
	
	MenuClass = MenuBPClass.Class;
}
void UPuzzlePlatformsGameInstance::Init()
{
	UE_LOG(LogTemp, Warning, TEXT("Found class %s"), *MenuClass->GetName());

}

void UPuzzlePlatformsGameInstance::Host()
{
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
	UEngine* Engine = GetEngine();
	if (!ensure(Engine != NULL)) return;

	Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, FString::Printf(TEXT("Joining %s"), *Adress));

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != NULL)) return;

	PlayerController->ClientTravel(*Adress, ETravelType::TRAVEL_Absolute);
}

