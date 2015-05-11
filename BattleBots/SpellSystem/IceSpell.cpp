// // Copyright 2015 VMR Games, Inc. All Rights Reserved.

#include "BattleBots.h"
#include "IceSpell.h"



float AIceSpell::ProcessElementalDmg(float initialDamage)
{
  if (GetSpellCaster()) {
    float dmgMod = 1 + FMath::Clamp(GetSpellCaster()->GetDamageModifier_Ice(), -1.f, 1.f);
    return FMath::Abs(initialDamage * dmgMod);
  }
  else {
    GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Caster is null - Ice"));
    return initialDamage;
  }
}

FDamageEvent& AIceSpell::GetDamageEvent()
{
  generalDamageEvent.DamageTypeClass = UBBotDmgType_Ice::StaticClass();
  return generalDamageEvent;
}