// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzlePlatformsGameInstance.h"

#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"

#include "OnlineSessionSettings.h"


#include <PuzzlePlatform/MainMenu.h>
#include <PuzzlePlatform/MenuWidget.h>

const static FName SESSION_NAME = TEXT("GameSession");
const static FName SESSION_NAME_SETING_KEY = TEXT("ServerName");



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
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem != nullptr)
	{
		
		UE_LOG(LogTemp, Warning, TEXT("Found subsystem %s"), *Subsystem->GetSubsystemName().ToString());
		SessionInterface = Subsystem->GetSessionInterface();
		if (SessionInterface.IsValid()) {
			
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnCreateSessionComplete);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnDestroySessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnFindSessionComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnJoinSessionComplete);
		}
	}	
	else
	{ 
		UE_LOG(LogTemp, Warning, TEXT("Didnt find subsystem"));
	}

	if (GEngine != nullptr)
	{
		GEngine->OnNetworkFailure().AddUObject(this,&UPuzzlePlatformsGameInstance::OnNetworkFailure);
	}
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



void UPuzzlePlatformsGameInstance::Host(FString ServerName)
{
	DesiredServerName = ServerName;
	if (SessionInterface.IsValid())
	{
		auto ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME);
		if (ExistingSession != nullptr) {
		
			SessionInterface->DestroySession(SESSION_NAME);
		}
		else
		{
			CreateSession();
		}
	}
}

void UPuzzlePlatformsGameInstance::RefreshServerList()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (SessionSearch.IsValid())
	{
		//SessionSearch->bIsLanQuery = true;
		SessionSearch->MaxSearchResults = 100;
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
		UE_LOG(LogTemp, Warning, TEXT("Starting find session"));
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}
}

void UPuzzlePlatformsGameInstance::OnFindSessionComplete(bool Success)
{
	
	if (Success && SessionSearch.IsValid() && Menu!=nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Sesions are found"));
		TArray<FServerData> ServerNames;

		for (const FOnlineSessionSearchResult& SearchResult : SessionSearch->SearchResults)
		{
			UE_LOG(LogTemp, Warning, TEXT("Found session names : %s"),*SearchResult.GetSessionIdStr());
			FServerData Data;
			Data.Name = SearchResult.GetSessionIdStr();
			Data.MaxPlayers = SearchResult.Session.SessionSettings.NumPublicConnections;
			Data.CurentPlayers = Data.MaxPlayers - SearchResult.Session.NumOpenPublicConnections;
			Data.HostUserName = SearchResult.Session.OwningUserName;
			FString ServerName;
			if (SearchResult.Session.SessionSettings.Get(SESSION_NAME_SETING_KEY, ServerName))
			{
				Data.Name = ServerName;
			}
			else
			{
				Data.Name = "Could not find name";
			}
			ServerNames.Add(Data);
		}

		Menu->SetServerList(ServerNames);
	}

}

void UPuzzlePlatformsGameInstance::OnDestroySessionComplete(FName SessionName, bool Success)
{
	if (Success)
	{
		CreateSession();
	}
}
void UPuzzlePlatformsGameInstance::OnNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString)
{
	LoadMainMenu();
}

void UPuzzlePlatformsGameInstance::CreateSession()
{
	if (SessionInterface.IsValid())
	{
		FOnlineSessionSettings SessionSettings;
		if (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL")
		{
			SessionSettings.bIsLANMatch = true;
		}
		else
		{
			SessionSettings.bIsLANMatch = false;
		}
		
		SessionSettings.NumPublicConnections = 5;
		SessionSettings.bShouldAdvertise = true;
		SessionSettings.bUsesPresence = true;
		SessionSettings.bUseLobbiesIfAvailable = true;
		SessionSettings.Set(SESSION_NAME_SETING_KEY, DesiredServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

		SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
	}
}


void UPuzzlePlatformsGameInstance::OnCreateSessionComplete(FName SessionName, bool Success)
{
	if (!Success)
	{

		UE_LOG(LogTemp, Warning, TEXT("Could not create session"));
		return;
	}
	if (Menu != nullptr)
	{
		Menu->Teardown();
	}

	UEngine* Engine = GetEngine();
	if (!ensure(Engine != NULL)) return;
	
	Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Hosting"));

	UWorld* World = GetWorld();
		if (!ensure(World != NULL)) return;

	World->ServerTravel("/Game/ThirdPerson/Maps/MP_Lobby?listen");

	
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

void UPuzzlePlatformsGameInstance::Join(uint32 Index)
{
	if (!SessionInterface.IsValid()) return;
	if (!SessionSearch.IsValid()) return;
	if (Menu != nullptr)
	{
	Menu->Teardown();
	}

	SessionInterface->JoinSession(0, SESSION_NAME, SessionSearch->SearchResults[Index]);

}
void UPuzzlePlatformsGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (!SessionInterface.IsValid()) return;
	FString Adress;
	if (!SessionInterface->GetResolvedConnectString(SessionName, Adress)) {
		UE_LOG(LogTemp, Warning, TEXT("Could not get connect String."));
		return;
	}

	UEngine* Engine = GetEngine();
	if (!ensure(Engine != NULL)) return;

	Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, FString::Printf(TEXT("Joining %s"), *Adress));

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != NULL)) return;

	PlayerController->ClientTravel(*Adress, ETravelType::TRAVEL_Absolute);
}
void UPuzzlePlatformsGameInstance::StartSession()
{
	if (SessionInterface.IsValid())
	{
		SessionInterface->StartSession(SESSION_NAME);
	}
}
void UPuzzlePlatformsGameInstance::LoadMainMenu()
{

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != NULL)) return;

	PlayerController->ClientTravel("/Game/ThirdPerson/Maps/MP_MainMenu", ETravelType::TRAVEL_Absolute);
}

