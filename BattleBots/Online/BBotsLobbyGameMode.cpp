// Copyright 2015 VMR Games, Inc. All Rights Reserved.

#include "BattleBots.h"
#include "Controllers/BBotsLobbyPC.h"
#include "BBotsLobbyPlayerState.h"
#include "BBotsLobbyGameState.h"
#include "BBotsLobbyGameMode.h"





ABBotsLobbyGameMode::ABBotsLobbyGameMode(const FObjectInitializer& ObjectInitializer)
  : Super(ObjectInitializer)
{
  PlayerControllerClass = ABBotsLobbyPC::StaticClass();
  PlayerStateClass = ABBotsLobbyPlayerState::StaticClass();
  GameStateClass = ABBotsLobbyGameState::StaticClass();
}

// Default implementation for game lobbies
bool ABBotsLobbyGameMode::ReadyToLoadMap()
{
  ABBotsLobbyGameState* const MyGameState = GetGameState<ABBotsLobbyGameState>();
  if (MyGameState)
  {
    return MyGameState->ReadyToLoadMap();
  }

  return false;
}

bool ABBotsLobbyGameMode::ReadyToEndMatch_Implementation()
{
  return ReadyToLoadMap();
}

void ABBotsLobbyGameMode::EndMatch()
{
  GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Magenta, TEXT("Map has ENDED! "));
  LoadNextMap();
}
