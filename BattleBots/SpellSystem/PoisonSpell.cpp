// // Copyright 2015 VMR Games, Inc. All Rights Reserved.

#include "BattleBots.h"
#include "PoisonSpell.h"


float APoisonSpell::ProcessElementalDmg(float initialDamage)
{
  if (GetSpellCaster()) {
    float dmgMod = 1 + FMath::Clamp(GetSpellCaster()->GetDamageModifier_Poison(), -1.f, 1.f);
    return FMath::Abs(initialDamage * dmgMod);
  }
  else {
    GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Caster is null - poison"));
    return initialDamage;
  }
}

FDamageEvent& APoisonSpell::GetDamageEvent()
{
  generalDamageEvent.DamageTypeClass = UBBotDmgType_Poison::StaticClass();
  return generalDamageEvent;
}