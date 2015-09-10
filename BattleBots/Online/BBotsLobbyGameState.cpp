// Copyright 2015 VMR Games, Inc. All Rights Reserved.

#include "BattleBots.h"
#include "Online/BBotsLobbyPlayerState.h"
#include "BBotsLobbyGameState.h"



void ABBotsLobbyGameState::AddReadyPlayer(APlayerState* playerReady)
{
  if (HasAuthority())
  {
    readyPlayers.AddUnique(playerReady);
  }
}

void ABBotsLobbyGameState::RemoveReadyPlayer(APlayerState* playerNotReady)
{
  if (HasAuthority())
  {
  	readyPlayers.Remove(playerNotReady);
  }
}

bool ABBotsLobbyGameState::ReadyToLoadMap()
{
  // If the number of ready players is equal the total number of players then we can load the game
  return PlayerArray.Num() == GetReadyPlayers().Num();
}

TArray<class APlayerState*> ABBotsLobbyGameState::GetReadyPlayers()
{
  return readyPlayers;
}
