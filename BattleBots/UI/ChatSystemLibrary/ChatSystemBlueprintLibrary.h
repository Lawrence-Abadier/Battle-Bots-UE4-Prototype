// Copyright 2015 VMR Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "ChatSystemBlueprintLibrary.generated.h"

#define LOCTEXT_NAMESPACE "DisplayableTimeStamp"

UENUM(BlueprintType)
enum class EMessageType :uint8{
  EAll        UMETA(DisplayName = "All"),
  ETeam       UMETA(DisplayName = "Team"),
  EWhisper    UMETA(DisplayName = "Whisper"),
  EUnkown     UMETA(DisplayName = "Unkown"),
};

USTRUCT(BlueprintType)
struct FTimeStamp{
  GENERATED_USTRUCT_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "ChatMessage")
    FDateTime CurrentDateStamp;
  UPROPERTY(BlueprintReadWrite, Category = "ChatMessage")
    int32 Hour;
  UPROPERTY(BlueprintReadWrite, Category = "ChatMessage")
    int32 Minutes;
  UPROPERTY(BlueprintReadWrite, Category = "ChatMessage")
    int32 Seconds;

  // Returns the displayable time stamp of the message
  FText GetTimeStampText(){
    CurrentDateStamp.Now();
    Hour = CurrentDateStamp.GetHour12();
    Minutes = CurrentDateStamp.GetMinute();
    Seconds = CurrentDateStamp.GetSecond();

    FText DisplayStamp = FText::Format(LOCTEXT("TimeStamp", "[{0}:{1}:{2}]"),
      FText::AsNumber(Hour),
      FText::AsNumber(Minutes),
      FText::AsNumber(Seconds));
    return DisplayStamp;
  }
};

// Holds chat message data
USTRUCT(BlueprintType)
struct FChatMessage{
  GENERATED_USTRUCT_BODY()

  UPROPERTY(BlueprintReadWrite, Category = "ChatMessage")
    FText Sender;
  UPROPERTY(BlueprintReadWrite, Category = "ChatMessage")
    FText Reciever;
  UPROPERTY(BlueprintReadWrite, Category = "ChatMessage")
    FText Message;
  UPROPERTY(BlueprintReadOnly, Category = "ChatMessage")
    FText MessageToDisplay;
  UPROPERTY(BlueprintReadWrite, Category = "ChatMessage")
    FSlateColor MessageColor;
  UPROPERTY(BlueprintReadWrite, Category = "ChatMessage")
    FText TimeStamp;
  UPROPERTY(BlueprintReadWrite, Category = "ChatMessage")
    int32 TeamNumber;
  UPROPERTY(BlueprintReadWrite, Category = "ChatMessage")
    EMessageType MessageType;
};

// Logs all chat messages relevant to the player
USTRUCT(BlueprintType)
struct FChatLog{
  GENERATED_USTRUCT_BODY()

  UPROPERTY(BlueprintReadWrite, Category = "ChatMessage")
  FChatMessage PreviousSender;
  UPROPERTY(BlueprintReadWrite, Category = "ChatMessage")
  TArray<FChatMessage> PreviousMessages;
};

/**
 * 
 */
UCLASS()
class BATTLEBOTS_API UChatSystemBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	
	
	
};

#undef LOCTEXT_NAMESPACE