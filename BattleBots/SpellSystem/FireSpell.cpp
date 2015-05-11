// // Copyright 2015 VMR Games, Inc. All Rights Reserved.

#include "BattleBots.h"
#include "FireSpell.h"





float AFireSpell::ProcessElementalDmg(float initialDamage)
{
  if (GetSpellCaster())
  {
	  float dmgMod = 1 + FMath::Clamp(GetSpellCaster()->GetDamageModifier_Fire(), -1.f, 1.f);
	  return FMath::Abs(initialDamage * dmgMod);
  }
  else {
    GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Caster is null - fire"));
    return initialDamage;
  }
}

FDamageEvent& AFireSpell::GetDamageEvent()
{
  generalDamageEvent.DamageTypeClass = UBBotDmgType_Fire::StaticClass();
  return generalDamageEvent;
}