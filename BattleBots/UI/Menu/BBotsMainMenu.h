// Copyright 2015 VMR Games, Inc. All Rights Reserved.

#pragma once

#include "Blueprint/UserWidget.h"
#include "BBotsMainMenu.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEBOTS_API UBBotsMainMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:

  // Host a game with the passed in type
  UFUNCTION(BlueprintCallable, Category = "Main Menu")
  void HostGame(const FString& GameType);
	
	
};
