// Copyright 2015 VMR Games, Inc. All Rights Reserved.

#include "BattleBots.h"
#include "BBotsGameState.h"





ABBotsGameState::ABBotsGameState(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
  numTeams = 0;
  remainingTime = 0;
  bTimerPaused = false;
  bRoundOver = false;
}

void ABBotsGameState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
  Super::GetLifetimeReplicatedProps(OutLifetimeProps);

  DOREPLIFETIME(ABBotsGameState, numTeams);
  DOREPLIFETIME(ABBotsGameState, remainingTime);
  DOREPLIFETIME(ABBotsGameState, bTimerPaused);
  DOREPLIFETIME(ABBotsGameState, teamScores);
}

