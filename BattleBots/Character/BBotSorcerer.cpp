// Copyright 2015 VMR Games, Inc. All Rights Reserved.

#include "BattleBots.h"
#include "BBotSorcerer.h"




void ABBotSorcerer::InitCombatStances()
{
  Super::InitCombatStances();

  if (Role == ROLE_Authority)
  {
	  // Init the 3 combat stances per inherited class
	  combatStances.Add(EStanceType::EFire);
	  combatStances.Add(EStanceType::EFrost);
	  combatStances.Add(EStanceType::ELightning);
	
	  SetCurrentStance(EStanceType::EFire);
  }
}

void ABBotSorcerer::printCurrentStance()
{
  EStanceType myStance = GetCurrentStance();

  switch (myStance)
  {
  case EStanceType::EFire: GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("FIRE!!!!!!"));
    break;
  case EStanceType::EFrost: GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("FROST!!"));
    break;
  case EStanceType::ELightning: GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("LIGHTNING!!"));
    break;
  default:
    GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("No stance????"));
    break;
  }
}

void ABBotSorcerer::OnRep_StanceChanged()
{
  // @todo:Implement stance table
}
