// Copyright 2015 VMR Games, Inc. All Rights Reserved.

#pragma once

#include "GameFramework/GameSession.h"
#include "BBotsGameSession.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEBOTS_API ABBotsGameSession : public AGameSession
{
	GENERATED_BODY()
	
public:

  /** Default number of players allowed in a game */
  static const int32 DEFAULT_NUM_PLAYERS = 6;

  /**
  * Host a new online session
  *
  * @param UserId user that initiated the request
  * @param SessionName name of session
  * @param bIsLAN is this going to hosted over LAN
  * @param bIsPresence is the session to create a presence session
  * @param MaxNumPlayers Maximum number of players to allow in the session
  *
  * @return bool true if successful, false otherwise
  */
  //UFUNCTION(BlueprintCallable, Category = "Sessions")
  bool HostSession(TSharedPtr<FUniqueNetId> UserId, FName SessionName, const FString& GameType, const FString& MapName, bool bIsLAN, bool bIsPresence, int32 MaxNumPlayers);
	
	

  /** Handles to various registered delegates */
  FDelegateHandle OnStartSessionCompleteDelegateHandle;
  FDelegateHandle OnCreateSessionCompleteDelegateHandle;
  FDelegateHandle OnDestroySessionCompleteDelegateHandle;
  FDelegateHandle OnFindSessionsCompleteDelegateHandle;
  FDelegateHandle OnJoinSessionCompleteDelegateHandle;
};
