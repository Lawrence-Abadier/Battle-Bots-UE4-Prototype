// Copyright 2015 VMR Games, Inc. All Rights Reserved.

#include "BattleBots.h"
#include "BBotsGameInstance.h"





bool UBBotsGameInstance::HostGame(ULocalPlayer* LocalPlayer, const FString& GameType, const FString& InTravelURL)
{
  if (!GetIsOnline())
  {
    //
    // Offline game, just go straight to map
    //
    // Travel to the specified match URL
    GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, TEXT("HostGame_GAMEINSTANCE"));
    TravelURL = InTravelURL;
    GetWorld()->ServerTravel(TravelURL);
    return true;
  }

  //
  // Online game
  //

  ABBotsGameSession* const GameSession = GetGameSession();
  if (GameSession)
  {
    TravelURL = InTravelURL;
    bool const bIsLanMatch = InTravelURL.Contains(TEXT("?bIsLanMatch"));

    //determine the map name from the travelURL
    const FString& MapNameSubStr = "/Game/Maps/";
    const FString& ChoppedMapName = TravelURL.RightChop(MapNameSubStr.Len());
    const FString& MapName = ChoppedMapName.LeftChop(ChoppedMapName.Len() - ChoppedMapName.Find("?game"));

    if (GameSession->HostSession(LocalPlayer->GetPreferredUniqueNetId(), GameSessionName, GameType, MapName, true/*bIsLanMatch*/, true, ABBotsGameSession::DEFAULT_NUM_PLAYERS))
    {
        return true;
    }
  }

  return false;
}

ABBotsGameSession* UBBotsGameInstance::GetGameSession()
{
  UWorld* const World = GetWorld();
  if (World)
  {
    AGameMode* const Game = World->GetAuthGameMode();
    if (Game)
    {
      return Cast<ABBotsGameSession>(Game->GameSession);
    }
  }

  return nullptr;
}
