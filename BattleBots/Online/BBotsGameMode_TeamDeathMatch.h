// Copyright 2015 VMR Games, Inc. All Rights Reserved.

#pragma once

#include "BattleBotsGameMode.h"
#include "BBotsGameMode_TeamDeathMatch.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEBOTS_API ABBotsGameMode_TeamDeathMatch : public ABattleBotsGameMode
{
	GENERATED_BODY()

public:
  UFUNCTION(BlueprintCallable)
  void SelectTeam(ABBotsPlayerState* player, int32 teamNum);
	
protected:
  int32 winningTeam;

  // Sets the winning team
  virtual void DetermineMatchWinner() override;
	
};
