// Copyright 2015 VMR Games, Inc. All Rights Reserved.

#pragma once

#include "GameFramework/PlayerStart.h"
#include "Interfaces/BBotsTeamInterface.h"
#include "BBotsPlayerStart.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEBOTS_API ABBotsPlayerStart : public APlayerStart, public IBBotsTeamInterface
{
	GENERATED_BODY()
	
public:
  virtual uint8 GetTeamNum() const
  {
    return TeamNum;
  }

protected:
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStart")
  uint8 TeamNum;
	
};
