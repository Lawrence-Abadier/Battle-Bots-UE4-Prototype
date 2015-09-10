// Copyright 2015 VMR Games, Inc. All Rights Reserved.

#pragma once

#include "Online/BBotsBaseGameMode.h"
#include "BBotsLobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEBOTS_API ABBotsLobbyGameMode : public ABBotsBaseGameMode
{
	GENERATED_BODY()
	
public:
  ABBotsLobbyGameMode(const FObjectInitializer& ObjectInitializer);
	
protected:
  // Ready when all players click ready in the game lobby
  virtual bool ReadyToEndMatch_Implementation() override;

  // Run loading screen and load next map
  virtual void EndMatch() override;

  UFUNCTION(BlueprintCallable, Category = "GameLobby")
  virtual bool ReadyToLoadMap() override;
};
