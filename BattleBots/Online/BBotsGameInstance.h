// Copyright 2015 VMR Games, Inc. All Rights Reserved.

#pragma once

#include "Engine/GameInstance.h"
#include "BBotsGameSession.h"
#include "BBotsGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEBOTS_API UBBotsGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
  // Returns true if the game is online
  FORCEINLINE bool GetIsOnline() const { return bIsOnline; }

  ABBotsGameSession* GetGameSession();

  bool HostGame(ULocalPlayer* LocalPlayer, const FString& GameType, const FString& InTravelURL);
private:
  /** URL to travel to after pending network operations */
  FString TravelURL;

  // Whether the match is online or not
  bool bIsOnline;
};
