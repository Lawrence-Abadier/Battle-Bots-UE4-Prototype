// Copyright 2015 VMR Games, Inc. All Rights Reserved.

#pragma once

#include "BBotsPlayerState.h"
#include "GameFramework/GameState.h"
#include "BBotsGameState.generated.h"

/** ranked PlayerState map, created from the GameState */
typedef TMap<int32, TWeakObjectPtr<ABBotsPlayerState> > RankedPlayerMap;

/**
 * 
 */
UCLASS()
class BATTLEBOTS_API ABBotsGameState : public AGameState
{
	GENERATED_BODY()
	
public:
  ABBotsGameState(const FObjectInitializer& ObjectInitializer);

  // Returns the total number of rounds this match
  FORCEINLINE int32 GetRoundsThisMatch() const{ return totalNumRounds; }
  // Increments the total number of rounds this match
  FORCEINLINE void IncRoundsThisMatch() { totalNumRounds++; }

  FORCEINLINE bool IsWarmUpRound() { return totalNumRounds == 0; }

  /** number of teams in current game (doesn't deprecate when no players are left in a team) */
  UPROPERTY(Transient, Replicated)
  int32 numTeams;

  /** accumulated score per team */
  UPROPERTY(Transient, Replicated)
  TArray<int32> teamScores;

  // The total number of rounds
  int32 totalNumRounds;

  /** time left for warmup / match */
  UPROPERTY(Transient, Replicated)
  int32 remainingTime;

  /** is timer paused? */
  UPROPERTY(Transient, Replicated)
  bool bTimerPaused;
};
