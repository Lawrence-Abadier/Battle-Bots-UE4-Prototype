// Copyright 2015 VMR Games, Inc. All Rights Reserved.

#include "BattleBots.h"
#include "BBotsGameMode_FreeForAll.h"





void ABBotsGameMode_FreeForAll::DetermineMatchWinner()
{
  ABBotsGameState* const MyGameState = Cast<ABBotsGameState>(GameState);

  if (MyGameState)
  {
    winningPlayer = Cast<ABBotsPlayerState>(FMath::Max<APlayerState>(MyGameState->PlayerArray));
  }
}
