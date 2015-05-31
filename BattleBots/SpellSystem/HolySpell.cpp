// // Copyright 2015 VMR Games, Inc. All Rights Reserved.

#include "BattleBots.h"
#include "HolySpell.h"





void AHolySpell::PostInitializeComponents()
{
  Super::PostInitializeComponents();

  if (HasAuthority())
  {
    defaultDamageEvent.DamageTypeClass = UBBotDmgType_Holy::StaticClass();
  }
}

float AHolySpell::ProcessElementalDmg(float initialDamage)
{
  if (GetSpellCaster()) {
    float dmgMod = 1 + FMath::Clamp(GetSpellCaster()->GetDamageModifier_Holy(), -1.f, 1.f);
    return FMath::Abs(initialDamage * dmgMod);
  }
  else {
    GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Caster is null - holy"));
    return initialDamage;
  }
}

FDamageEvent& AHolySpell::GetDamageEvent()
{
  return defaultDamageEvent;
}