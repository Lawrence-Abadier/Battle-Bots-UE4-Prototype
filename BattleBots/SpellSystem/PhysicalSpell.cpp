// // Copyright 2015 VMR Games, Inc. All Rights Reserved.

#include "BattleBots.h"
#include "PhysicalSpell.h"





void APhysicalSpell::PostInitializeComponents()
{
  Super::PostInitializeComponents();

  if (HasAuthority())
  {
    defaultDamageEvent.DamageTypeClass = UBBotDmgType_Physical::StaticClass();
  }
}

float APhysicalSpell::ProcessElementalDmg(float initialDamage)
{
  if (GetSpellCaster()) {
    float dmgMod = 1 + FMath::Clamp(GetSpellCaster()->GetDamageModifier_Physical(), -1.f, 1.f);
    return FMath::Abs(initialDamage * dmgMod);
  }
  else {
    GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Caster is null - physical"));
    return initialDamage;
  }
}

FDamageEvent& APhysicalSpell::GetDamageEvent()
{
  return defaultDamageEvent;
}