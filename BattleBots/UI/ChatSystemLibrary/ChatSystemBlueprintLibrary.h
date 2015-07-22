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
    CurrentDateStamp = FDateTime::Now();
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
struct FChatMessageStruct{
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
  FText LastSender;
  UPROPERTY(BlueprintReadWrite, Category = "ChatMessage")
  TArray<FChatMessageStruct> PreviousMessages;
};

/**
 * 
 */
UCLASS()
class BATTLEBOTS_API UChatSystemBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

    /** Retrieves the OS system Date and Time as a string at the instant this BP node runs. Use my other RealWorldTime node to get the time passed since the return value of this node! You can use this to record milliseconds/seconds/minutes/hours between events in game logic! */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VictoryBPLibrary")
    static FString RealWorldTime__GetCurrentOSTime(
    int32& MilliSeconds,
    int32& Seconds,
    int32& Minutes,
    int32& Hours12,
    int32& Hours24,
    int32& Day,
    int32& Month,
    int32& Year
    );
	
	
};

#undef LOCTEXT_NAMESPACE