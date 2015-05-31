// // Copyright 2015 VMR Games, Inc. All Rights Reserved.

#pragma once

#include "SpellSystem/DamageTypes/BBotDmgType_Lightning.h"
#include "SpellSystem/SpellSystem.h"
#include "LightningSpell.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEBOTS_API ALightningSpell : public ASpellSystem
{
	GENERATED_BODY()
	
public:
  // Called after all components have been initialized with default values
  virtual void PostInitializeComponents() override;

  // Returns the damage event and type
  virtual FDamageEvent& GetDamageEvent() override;

protected:
  // Processes final elemental damage post item dmg modifiers
  virtual float ProcessElementalDmg(float initialDamage) override;
};
