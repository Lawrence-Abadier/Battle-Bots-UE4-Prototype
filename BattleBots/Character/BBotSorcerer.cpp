// Copyright 2015 VMR Games, Inc. All Rights Reserved.

#include "BattleBots.h"
#include "BBotSorcerer.h"




void ABBotSorcerer::InitCombatStances()
{
  if (HasAuthority()) {
    // Init the 3 combat stances per inherited class
    combatStances.Add(EStanceType::EFire);
    combatStances.Add(EStanceType::EFrost);
    combatStances.Add(EStanceType::ELightning);

    //SetCurrentStance(EStanceType::EFire);
  }
}

void ABBotSorcerer::printCurrentStance()
{
  //   EStanceType myStance = GetCurrentStance();
  // 
  //   switch (myStance) {
  //     case EStanceType::EFire: GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("FIRE!!!!!!"));
  //       break;
  //     case EStanceType::EFrost: GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("FROST!!"));
  //       break;
  //     case EStanceType::ELightning: GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("LIGHTNING!!"));
  //       break;
  //     default:
  //       GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("No stance????"));
  //       break;
  //   }
}

void ABBotSorcerer::OnRep_StanceChanged()
{
  if (Role < ROLE_Authority)
  {
    ServerOnRep_StanceChanged();
  }
  else
  {
    // @todo:Implement stance table
    EStanceType myStance = GetCurrentStance();

    switch (myStance) {
      case EStanceType::EFire:
        SetToDamageStance();
        break;
      case EStanceType::EFrost:
        SetToDefenseStance();
        break;
      case EStanceType::ELightning:
        SetToMobilityStance();
        break;
      default:
        GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("No stance????"));
        break;
    }
  }
}

void ABBotSorcerer::SetToMobilityStance()
{
  if (Role == ROLE_Authority)
  {
    // Increase Speed by 60%
    SetMobilityModifier_All(0.6f);
    // Reduce Resists and Block by 20%
    SetDefenseModifier_All(-0.2f);
    // Reduce Damage by 20%
    SetDamageModifier_All(-0.2f);

    GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Green, TEXT("LIGHTNING!! - Mobility Stance- ") + FString::FromInt(100 * characterConfig.bonusFireDmg));
  }
}

void ABBotSorcerer::SetToDamageStance()
{
  if (Role == ROLE_Authority)
  {
    // Reduce Speed by 20%
    SetMobilityModifier_All(-0.2f);
    // Reduce Resists and Block by 20%
    SetDefenseModifier_All(-0.2f);
    // Increase Damage by 60%
    SetDamageModifier_All(0.6f);

    GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Green, TEXT("FIRE!!!!!! - Damage Stance- ") + FString::FromInt(100 * characterConfig.bonusFireDmg));
  }
}

void ABBotSorcerer::SetToDefenseStance()
{
  if (Role == ROLE_Authority)
  {
    // Reduce Speed by 20%
    SetMobilityModifier_All(-0.2f);
    // Increase Resists and Block by 60%
    SetDefenseModifier_All(0.6f);
    // Reduce Damage by 20%
    SetDamageModifier_All(-0.2f);

    GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Green, TEXT("FROST!! - Ice Stance- ") + FString::FromInt(100 * characterConfig.bonusFireDmg));
  }
}

// NOT CHANGING VALUE AFTER INITIAL CALL
void ABBotSorcerer::SetDamageModifier_All(float newDmgMod)
{
  if (HasAuthority()) {
    // Resets current mod, need an item config struct
    characterConfig.bonusFireDmg = FMath::Clamp(GetDefaultCharConfigValues().fireResist + newDmgMod, -1.f, 1.f);
    characterConfig.bonusLightningDmg = FMath::Clamp(GetDefaultCharConfigValues().bonusLightningDmg + newDmgMod, -1.f, 1.f);
    characterConfig.bonusIceDmg = FMath::Clamp(GetDefaultCharConfigValues().bonusIceDmg + newDmgMod, -1.f, 1.f);
  }
}
