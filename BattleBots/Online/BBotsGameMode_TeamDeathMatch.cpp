// Copyright 2015 VMR Games, Inc. All Rights Reserved.

#include "BattleBots.h"
#include "BBotsGameState.h"
#include "BBotsGameMode_TeamDeathMatch.h"



void ABBotsGameMode_TeamDeathMatch::SelectTeam(ABBotsPlayerState* player, int32 teamNum)
{
  player->SetTeamNum(teamNum);
}

void ABBotsGameMode_TeamDeathMatch::DetermineMatchWinner()
{
  ABBotsGameState* const MyGameState = Cast<ABBotsGameState>(GameState);

  if (MyGameState)
  {
    // TeamScores are indexed by teamNumber, so we only need to return the max index
    FMath::Max<int32>(MyGameState->teamScores, &winningTeam);
  }
}
