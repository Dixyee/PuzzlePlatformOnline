// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	++NumberOfPlayers;

	if (NumberOfPlayers >= 3)
	{
		UWorld* World = GetWorld();
		if (!ensure(World != NULL)) return;

		bUseSeamlessTravel = true;
		World->ServerTravel("/Game/ThirdPerson/Maps/MP_PP_Level1?listen");
		UE_LOG(LogTemp, Warning, TEXT("Reached 3 players"));
	}
}
void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	--NumberOfPlayers;
}