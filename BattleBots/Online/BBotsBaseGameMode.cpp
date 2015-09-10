// Copyright 2015 VMR Games, Inc. All Rights Reserved.

#include "BattleBots.h"
#include "BBotsBaseGameMode.h"





FString ABBotsBaseGameMode::GetNextMap()
{
  return postGameMapName;
}

void ABBotsBaseGameMode::SetNextMap(FString NextMap)
{
  if (HasAuthority())
  {
  	postGameMapName = NextMap;
  }
}

void ABBotsBaseGameMode::LoadNextMap()
{
  FString serverTag = "?listen";
  FString mapDirectory = "/Game/TopDown/Maps/";
  FString postGameLobbyURL = mapDirectory.Append(GetNextMap()).Append(serverTag);
  // @Todo: If no nextmap available, load main menu without serverTag
  GetWorld()->ServerTravel(*postGameLobbyURL);
}

bool ABBotsBaseGameMode::ReadyToLoadMap()
{
  // must be overriden
  return false;
}
