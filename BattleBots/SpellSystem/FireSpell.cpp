// // Copyright 2015 VMR Games, Inc. All Rights Reserved.

#include "BattleBots.h"
#include "FireSpell.h"





float AFireSpell::ProcessElementalDmg(float initialDamage)
{
	  if (GetSpellCaster())
	  {
	    float dmgMod = 1 + FMath::Clamp(GetSpellCaster()->GetDamageModifier_Fire(), -1.f, 1.f);
	    GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Yellow, FString::FromInt(100 * dmgMod));
	    return FMath::Abs(initialDamage * dmgMod);
	  }
	  else {
	    GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Fire Caster Is null!!"));
	    return initialDamage;
	  }
}

FDamageEvent& AFireSpell::GetDamageEvent()
{
  generalDamageEvent.DamageTypeClass = UBBotDmgType_Fire::StaticClass();
  return generalDamageEvent;
}

// UDamageType* AFireSpell::GetDamageType()
// {
//   UBBotDmgType_Fire* fireDamageType;
//   return fireDamageType;
// }
