// Copyright 2015 VMR Games, Inc. All Rights Reserved.

#pragma once

#include "Online/BBotsGameState.h"
#include "BBotsLobbyGameState.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEBOTS_API ABBotsLobbyGameState : public ABBotsGameState
{
	GENERATED_BODY()
	
public:
  // Is map ready to be loaded?
  UFUNCTION(BlueprintCallable, Category = "GameLobby")
  virtual bool ReadyToLoadMap();

  UFUNCTION(BlueprintCallable, Category = "GameLobby")
  TArray<class APlayerState*> GetReadyPlayers();

  UFUNCTION(BlueprintCallable, Category = "GameLobby")
  void AddReadyPlayer(class APlayerState* playerReady);
  UFUNCTION(BlueprintCallable, Category = "GameLobby")
  void RemoveReadyPlayer(class APlayerState* playerNotReady);

private:
  
  // Container of all the ready players in the lobby
  UPROPERTY()
  TArray<class APlayerState*> readyPlayers;
	
};
