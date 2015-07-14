// Copyright 2015 VMR Games, Inc. All Rights Reserved.

#include "BattleBots.h"
#include "Online/BBotsPlayerState.h"
#include "Runtime/UMG/Public/Blueprint/WidgetTree.h"
#include "ChatBlockWidget.h"


#define LOCTEXT_NAMESPACE "MessageToDisplay" 


UChatBlockWidget::UChatBlockWidget(const FObjectInitializer& ObjectInitializer)
  : Super(ObjectInitializer)
{
  // Initialization list is not available for UE4, this is a workaround
  FString Types[] = { "/a", "/t", "/w", "/all", "/whisper", "/team" };
  MessageTypes.Append(Types, ARRAY_COUNT(Types));
}

void UChatBlockWidget::PostInitProperties()
{
  Super::PostInitProperties();
  FTimerHandle InitWidgetHandler;
  const UWorld* World = GetWorld();
  if (World)
  {
    World->GetTimerManager().SetTimer(InitWidgetHandler, this, &UChatBlockWidget::InitWidgetPostProp, 1, false);
  }
}

void UChatBlockWidget::InitWidgetPostProp()
{
  APlayerController* PC = GetOwningPlayer();
  if (PC)
  {
    CState = Cast<ABBotsPlayerState>(PC->PlayerState);
    PName = FText::FromString(CState->PlayerName);
  }
}

void UChatBlockWidget::LogMessage(FChatMessage Message)
{
  if (PName.EqualToCaseIgnored(Message.Sender))
  {
    // Log this character's messages only
    ChatLog.PreviousMessages.Add(Message);
    // Update the chat log index
    ChatLogIndex = ChatLog.PreviousMessages.Num()-1;
    CurrentChatLogIndex = ChatLogIndex;
  }
  else if (PName.EqualToCaseIgnored(Message.Reciever))
  {
    // Log the sender's name when whispered
    ChatLog.LastSender = Message.Sender;
    GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("ADDING LAST SENDER"));
  }
}

FText UChatBlockWidget::ReplyToLastSender()
{
  if (!ChatLog.LastSender.IsEmpty())
  {
    FText FormatedReplyMessage = FText::Format(LOCTEXT("MessageToDisplay", "/Whisper {0} "), ChatLog.LastSender);
    return FormatedReplyMessage;
  }
  return FText::GetEmpty();
}

FText UChatBlockWidget::GetLastMessageSent()
{
  FChatMessage TempMsg;

  if (ChatLog.PreviousMessages.IsValidIndex(CurrentChatLogIndex))
  {
    if (bUpKeyPressed)
    {
      TempMsg = ChatLog.PreviousMessages[CurrentChatLogIndex--];
      CurrentChatLogIndex = FMath::Clamp(CurrentChatLogIndex, 0, ChatLogIndex);
    }
    else
    {
      TempMsg = ChatLog.PreviousMessages[CurrentChatLogIndex++];
      CurrentChatLogIndex = FMath::Clamp(CurrentChatLogIndex, 0, ChatLogIndex);
    }
  }
  
  FText FormatedReplyMessage = FText::Format(LOCTEXT("MessageToDisplay", "{0} {1} "), MessageTypeToText(TempMsg.MessageType), TempMsg.Message);
  
  return FormatedReplyMessage;
}

FChatMessage UChatBlockWidget::RecieveMessage_Implementation(FChatMessage MessageRecieved)
{
  //@todo fix copy operator,bug?
  //MessageRecieved.TimeStamp = TimeStamp.GetTimeStampText();
  FText FormatArgs;
  bool bIsSender = false;

  switch (MessageRecieved.MessageType)
  {
  case EMessageType::EWhisper:
    // Change to whisper color
    MessageRecieved.MessageColor = WhisperChatMessageColor;

    if (MessageRecieved.Sender.EqualToCaseIgnored(PName))
    {
      bIsSender = true;
      FormatArgs = LOCTEXT("MessageToDisplay", "{0} [Whisper to {1}]: {2}");
    }
    else
    {
      FormatArgs = LOCTEXT("MessageToDisplay", "{0} [Whisper from {1}]: {2}");
    }
    break;

  case EMessageType::ETeam:
    // Change to team color
    MessageRecieved.MessageColor = TeamChatMessageColor;
    FormatArgs = LOCTEXT("MessageToDisplay", "{0} [Team] {1}: {2}");
    break;

  case EMessageType::EAll:
  default:
    // All other cases get sent to all chat
    MessageRecieved.MessageColor = AllChatMessageColor;
    FormatArgs = LOCTEXT("MessageToDisplay", "{0} [All] {1}: {2}");
    break;
  }

  FText MessageToDisplay = FText::Format(FormatArgs,
    TimeStamp.GetTimeStampText(),
    bIsSender ? MessageRecieved.Reciever
    : MessageRecieved.Sender,
    MessageRecieved.Message);

  MessageRecieved.MessageToDisplay = MessageToDisplay;

  return MessageRecieved;
}


bool UChatBlockWidget::CanRecieveMessage_Implementation(const FChatMessage& MessageRecieved)
{
  switch (MessageRecieved.MessageType)
  {
  case EMessageType::EAll:
    return true;
    break;
  case EMessageType::ETeam:
    return MessageRecieved.TeamNumber == CState->GetTeamNum();
    break;
  case EMessageType::EWhisper:
    return !MessageRecieved.Reciever.EqualToCaseIgnored(MessageRecieved.Sender);
    break;
  default:
    break;
  }
  return false;
}


FChatMessage UChatBlockWidget::ParseMessageData(const FString& UnParsedMessage)
{
  // We reset the struct data every message. See ChatMessage for more details.
  FChatMessage TempChatMessage;
  ChatMessage = TempChatMessage;

  // Set the sender to this character's name and TeamNumber
  ChatMessage.Sender = PName;
  ChatMessage.TeamNumber = CState->GetTeamNum();

  FString ParsedMessage;
  FString ChatCommand;
  FString Reciever;
  int32 IndexZero;

  // Did the user type in a command?
  if (UnParsedMessage.FindChar(TEXT('/'), IndexZero) && IndexZero == 0)
  {
    UnParsedMessage.Split(TEXT(" "), &ChatCommand, &ParsedMessage);

    if (!MessageTypes.Contains(ChatCommand.ToLower()))
    {
      //Returns an error msg for invalid command
      FString InvalidMessage = FString::Printf(TEXT("%s is an invalid command"), *ChatCommand);
      ChatMessage.Message = FText::FromString(InvalidMessage);

      return ChatMessage;
    }

    // Get message type
    ChatMessage.MessageType = StringToMessageType(ChatCommand);

    if (ChatCommand.ToLower() == "/w" || ChatCommand.ToLower() == "/whisper")
    {
      // Split the whisper command to get the target player and msg
      ParsedMessage.Split(TEXT(" "), &Reciever, &ParsedMessage);
      ChatMessage.Reciever = FText::FromString(Reciever);
      ChatMessage.Message = FText::FromString(ParsedMessage);

      return ChatMessage;
    }
    else
    {
      // If the player is not sending a whisper then the remaining text is a msg
      ChatMessage.Message = FText::FromString(ParsedMessage);

      return ChatMessage;
    }
  }

  // Returns the string if the user did not use any commands
  ChatMessage.MessageType = EMessageType::EAll;
  ChatMessage.Message = FText::FromString(UnParsedMessage);
  return ChatMessage;
}

EMessageType UChatBlockWidget::StringToMessageType(const FString& MessageCommand)
{
  FString Command = MessageCommand.ToLower();

  if (Command == "/a" || Command == "/all")
  {
    return EMessageType::EAll;
  }
  else if (Command == "/t" || Command == "/team")
  {
    return EMessageType::ETeam;
  }
  else if (Command == "/w" || Command == "/whisper")
  {
    return EMessageType::EWhisper;
  }

  return EMessageType::EUnkown;
}

FText UChatBlockWidget::MessageTypeToText(const EMessageType& MessageType)
{
  switch (MessageType)
  {
  case EMessageType::ETeam:
    return FText::FromString("/Team");
  case EMessageType::EWhisper:
    return FText::FromString("/Whisper");
  default:
    break;
  }
  return FText::FromString("/All");
}

#define LOCTEXT_NAMESPACE