// Copyright 2015 VMR Games, Inc. All Rights Reserved.

#pragma once

#include "Engine/GameInstance.h"
#include "BBotsGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEBOTS_API UBBotsGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
  // Returns the total number of rounds this match
  FORCEINLINE int32 GetRoundsThisMatch() const{ return totalNumRounds; }
  // Increments the total number of rounds this match
  FORCEINLINE void IncRoundsThisMatch() { totalNumRounds++; }

private:
  // The total number of rounds
  int32 totalNumRounds;
	
};
