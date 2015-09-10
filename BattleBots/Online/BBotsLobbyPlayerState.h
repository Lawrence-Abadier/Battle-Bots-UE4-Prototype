// Copyright 2015 VMR Games, Inc. All Rights Reserved.

#pragma once

#include "Online/BBotsPlayerState.h"
#include "BBotsLobbyPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEBOTS_API ABBotsLobbyPlayerState : public ABBotsPlayerState
{
	GENERATED_BODY()
	
public:
  UFUNCTION(BlueprintCallable, Category = "Player State")
  bool PlayerIsReady();
  
  // Sets the player to ready when they click ready on Lobby UI.
  UFUNCTION(BlueprintCallable, Category = "Player State")
  void SetPlayerIsReady();	

  // Sets the player to not-ready when they click not-ready on Lobby UI.
  UFUNCTION(BlueprintCallable, Category = "Player State")
  void SetPlayerNotReady();

private:

  // Since we are not replicating readyPlayers array, we must make the call on the server only
  UFUNCTION(Reliable, Server, WithValidation)
  void ServerPlayerIsReady();
  void ServerPlayerIsReady_Implementation();
  bool ServerPlayerIsReady_Validate();
  UFUNCTION(Reliable, Server, WithValidation)
  void ServerPlayerNotReady();
  void ServerPlayerNotReady_Implementation();
  bool ServerPlayerNotReady_Validate();
};
