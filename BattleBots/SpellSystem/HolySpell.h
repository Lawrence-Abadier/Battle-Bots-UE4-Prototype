// // Copyright 2015 VMR Games, Inc. All Rights Reserved.

#pragma once

#include "SpellSystem/DamageTypes/BBotDmgType_Holy.h"
#include "SpellSystem/SpellSystem.h"
#include "HolySpell.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEBOTS_API AHolySpell : public ASpellSystem
{
	GENERATED_BODY()
	
public:
  // Returns the damage event and type
  virtual FDamageEvent& GetDamageEvent() override;

protected:
  FDamageEvent generalDamageEvent;

  // Processes final elemental damage post item dmg modifiers
  virtual float ProcessElementalDmg(float initialDamage) override;
};
