// Copyright 2015 VMR Games, Inc. All Rights Reserved.

#pragma once

#include "GameFramework/GameMode.h"
#include "BBotsBaseGameMode.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEBOTS_API ABBotsBaseGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
  UFUNCTION(BlueprintCallable, Category = "Game Map")
  FString GetNextMap();
  UFUNCTION(BlueprintCallable, Category = "Game Map")
  void SetNextMap(FString NextMap);

  // Loads the next map
  UFUNCTION(BlueprintCallable, Category = "Game Map")
  virtual void LoadNextMap();

protected:  
  // Can we load next map?
  virtual bool ReadyToLoadMap();

  // The post game map name that is loaded after the match has ended.
  UPROPERTY(EditDefaultsOnly, Category = "Lobby")
  FString postGameMapName;
};
