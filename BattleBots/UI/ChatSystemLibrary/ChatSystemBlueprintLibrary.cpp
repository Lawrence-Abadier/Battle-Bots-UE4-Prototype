// Copyright 2015 VMR Games, Inc. All Rights Reserved.

#include "BattleBots.h"
#include "ChatSystemBlueprintLibrary.h"




FString UChatSystemBlueprintLibrary::RealWorldTime__GetCurrentOSTime(
  int32& MilliSeconds,
  int32& Seconds,
  int32& Minutes,
  int32& Hours12,
  int32& Hours24,
  int32& Day,
  int32& Month,
  int32& Year
  ){
  const FDateTime Now = FDateTime::Now();

  MilliSeconds = Now.GetMillisecond();
  Seconds = Now.GetSecond();
  Minutes = Now.GetMinute();
  Hours12 = Now.GetHour12();
  Hours24 = Now.GetHour(); //24
  Day = Now.GetDay();
  Month = Now.GetMonth();
  Year = Now.GetYear();

  return Now.ToString();
}