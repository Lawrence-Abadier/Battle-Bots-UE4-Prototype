// Copyright 2015 VMR Games, Inc. All Rights Reserved.

#include "BattleBots.h"
#include "BBotsLobbyPlayerState.h"





bool ABBotsLobbyPlayerState::PlayerIsReady()
{
  ABBotsLobbyGameState* const MyGameState = Cast<ABBotsLobbyGameState>(GetWorld()->GetGameState());
  if (MyGameState)
  {
    return MyGameState->GetReadyPlayers().Contains(this);
  }

  return false;
}

void ABBotsLobbyPlayerState::SetPlayerIsReady()
{
  ServerPlayerIsReady();
}

void ABBotsLobbyPlayerState::ServerPlayerIsReady_Implementation()
{
  ABBotsLobbyGameState* const MyGameState = Cast<ABBotsLobbyGameState>(GetWorld()->GetGameState());

  if (MyGameState)
  {
    MyGameState->AddReadyPlayer(this);
  }
}

bool ABBotsLobbyPlayerState::ServerPlayerIsReady_Validate()
{
  return true;
}

void ABBotsLobbyPlayerState::SetPlayerNotReady()
{
  ServerPlayerNotReady();
}

void ABBotsLobbyPlayerState::ServerPlayerNotReady_Implementation()
{
  ABBotsLobbyGameState* const MyGameState = Cast<ABBotsLobbyGameState>(GetWorld()->GetGameState());

  if (MyGameState)
  {
    MyGameState->RemoveReadyPlayer(this);
  }
}

bool ABBotsLobbyPlayerState::ServerPlayerNotReady_Validate()
{
  return true;
}
