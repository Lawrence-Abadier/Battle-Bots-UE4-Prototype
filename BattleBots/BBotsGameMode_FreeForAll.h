// Copyright 2015 VMR Games, Inc. All Rights Reserved.

#pragma once

#include "BattleBotsGameMode.h"
#include "BBotsGameMode_FreeForAll.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEBOTS_API ABBotsGameMode_FreeForAll : public ABattleBotsGameMode
{
	GENERATED_BODY()
	
public:

  friend FORCEINLINE bool operator<(const APlayerState &PlayerOne, const APlayerState &PlayerTwo){
    return PlayerOne.Score < PlayerTwo.Score;
  }

protected:
  // The player with the highest score
  ABBotsPlayerState* winningPlayer;

  // Sets the winning player
  virtual void DetermineMatchWinner() override;
	
};
