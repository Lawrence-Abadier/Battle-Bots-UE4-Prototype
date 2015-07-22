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
