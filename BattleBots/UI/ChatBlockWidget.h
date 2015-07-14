// Copyright 2015 VMR Games, Inc. All Rights Reserved.

#pragma once

#include "Blueprint/UserWidget.h"
#include "ChatSystemLibrary/ChatSystemBlueprintLibrary.h"
#include "ChatBlockWidget.generated.h"

/**
 *
 */
UCLASS()
class BATTLEBOTS_API UChatBlockWidget : public UUserWidget
{
  GENERATED_BODY()

public:
  UChatBlockWidget(const FObjectInitializer& ObjectInitializer);

  virtual void PostInitProperties() override;
  
  // GetPlayerPawn is null in post prop and needs a delay
  void InitWidgetPostProp();

protected:
  
  // Player name
  UPROPERTY()
  FText PName;
  
  // The character playerstate
  UPROPERTY(BlueprintReadOnly, Category = "Chat")
  class ABBotsPlayerState* CState;

  // Sets the text color based on message type
  UPROPERTY(EditDefaultsOnly, Category = "ChatMessage")
  FSlateColor AllChatMessageColor;
  UPROPERTY(EditDefaultsOnly, Category = "ChatMessage")
  FSlateColor TeamChatMessageColor;
  UPROPERTY(EditDefaultsOnly, Category = "ChatMessage")
  FSlateColor WhisperChatMessageColor;

  /* Holds the chat message data. Requires UPROPERTY to be used with BP, 
  / hence why its in the header. Gets reset every msg.
  / ChatMessage has to be reset every time to prevent previous data from corrupting new msgs.*/
  UPROPERTY()
  FChatMessage ChatMessage;

  // Logs all text messages relevant to the player
  UPROPERTY(BlueprintReadOnly, Category = "ChatLog")
  FChatLog ChatLog;

  // True on up key press
  UPROPERTY(BlueprintReadWrite, Category = "ChatLog")
  bool bUpKeyPressed;

  // Returns the current timestamp of the message
  UPROPERTY(BlueprintReadWrite, Category = "TimeStamp")
  FTimeStamp TimeStamp;

  // Holds all the different message commands
  TArray<FString> MessageTypes;

  // Logs incoming messages for quick reply
  UFUNCTION(BlueprintCallable, Category = "ChatLog")
  void LogMessage(FChatMessage Message);

  // When a player presses R, returns the last sender
  UFUNCTION(BlueprintCallable, Category = "ChatLog")
  FText ReplyToLastSender();

  // When a player presses UP/Down Key, returns the last message
  UFUNCTION(BlueprintCallable, Category = "ChatLog")
  FText GetLastMessageSent();

  UFUNCTION(BlueprintCallable, Category = "Chat")
  FChatMessage ParseMessageData(const FString& UnParsedMessage);

  UFUNCTION(BlueprintCallable, Category = "Chat")
  EMessageType StringToMessageType(const FString& MessageCommand);

  UFUNCTION(BlueprintCallable, Category = "Chat")
  FText MessageTypeToText(const EMessageType& MessageType);

  UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Chat")
  FChatMessage RecieveMessage(FChatMessage MessageRecieved);
  FChatMessage RecieveMessage_Implementation(FChatMessage MessageRecieved);

  UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Chat")
  bool CanRecieveMessage(const FChatMessage& MessageRecieved);
  bool CanRecieveMessage_Implementation(const FChatMessage& MessageRecieved);

private:
  // Accesses chat log to return the next message
  UPROPERTY()
  int32 ChatLogIndex;

  UPROPERTY()
  int32 CurrentChatLogIndex;
};

