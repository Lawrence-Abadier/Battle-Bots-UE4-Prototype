// Copyright 2015 VMR Games, Inc. All Rights Reserved.

#pragma once

#include "BBotsTeamInterface.generated.h"

UINTERFACE(MinimalAPI)
class UBBotsTeamInterface : public UInterface
{
  GENERATED_UINTERFACE_BODY()
};


class BATTLEBOTS_API IBBotsTeamInterface
{
  GENERATED_IINTERFACE_BODY()

  virtual uint8 GetTeamNum() const PURE_VIRTUAL(IBBotsTeamInterface::GetTeamNum, return 255;);



};
