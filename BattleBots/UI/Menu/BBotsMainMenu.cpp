// Copyright 2015 VMR Games, Inc. All Rights Reserved.

#include "BattleBots.h"
#include "BBotsMainMenu.h"





void UBBotsMainMenu::HostGame(const FString& GameType)
{
  TWeakObjectPtr<UBBotsGameInstance> GameInstance = Cast<UBBotsGameInstance>(GetWorld()->GetGameInstance());
  GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, TEXT("HostGame_MENU"));
  TWeakObjectPtr<ULocalPlayer> PlayerOwner;
//   if (ensure(GameInstance.IsValid()) && PlayerOwner.Get() != NULL)
//   {
    FString const StartURL = "/Game/TopDown/Maps/TopDownExampleMap?listen";//FString::Printf(TEXT("/Game/Maps/%s?game=%s%s%s?%s=%d%s"), *GetMapName(), *GameType, GameInstance->GetIsOnline() ? TEXT("?listen") : TEXT(""), bIsLanMatch ? TEXT("?bIsLanMatch") : TEXT(""), *AShooterGameMode::GetBotsCountOptionName(), BotsCountOpt, bIsRecordingDemo ? TEXT("?DemoRec") : TEXT(""));
    GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, TEXT("HostGame_MENU"));
    // Game instance will handle success, failure and dialogs
    GameInstance->HostGame(PlayerOwner.Get(), GameType, StartURL);
/*  }*/
}
