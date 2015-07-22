// Copyright 2015 VMR Games, Inc. All Rights Reserved.

#include "BattleBots.h"
#include "BBotsGameSession.h"





bool ABBotsGameSession::HostSession(TSharedPtr<FUniqueNetId> UserId, FName SessionName, const FString& GameType, const FString& MapName, bool bIsLAN, bool bIsPresence, int32 MaxNumPlayers)
{
  GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, TEXT("HostGame_SESSION"));
  IOnlineSubsystem* const OnlineSub = IOnlineSubsystem::Get();
  if (OnlineSub)
  {
    TSharedPtr<FOnlineSessionSettings> HostSettings;
    IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
    if (Sessions.IsValid() && UserId.IsValid())
    {
      //HostSettings->Set
      //OnCreateSessionCompleteDelegateHandle = Sessions->AddOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegate);
      return Sessions->CreateSession(*UserId, SessionName, *HostSettings);
    }
  }
  return false;
}
