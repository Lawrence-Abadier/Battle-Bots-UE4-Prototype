// Copyright 2015 VMR Games, Inc. All Rights Reserved.

#pragma once

#include "BBotsResetInterface.generated.h"

// Implement this interface for Actors that should handle game mode resets (halftime, warm-up rounds, role swaps, etc)
UINTERFACE(MinimalAPI)
class UBBotsResetInterface : public UInterface
{
  GENERATED_UINTERFACE_BODY()
};


class BATTLEBOTS_API IBBotsResetInterface
{
  GENERATED_IINTERFACE_BODY()

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Game")
  void Reset();


};
