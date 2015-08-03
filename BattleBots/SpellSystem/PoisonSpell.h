// // Copyright 2015 VMR Games, Inc. All Rights Reserved.

#pragma once

#include "SpellSystem/DamageTypes/BBotDmgType_Poison.h"
#include "SpellSystem/SpellSystem.h"
#include "PoisonSpell.generated.h"

/**
  * APoisonSpell is the base class for a spell to be spawned in the level.
  * APoisonSpell objects are projectiles that spawn at spell caster location.
  * The projectiles get destroyed on contact. Enemies hit by the spell
  * receive a poison dot for X durations. The duration of the dot gets reset
  * to full duration if the enemy is hit before the duration ends.
 */
UCLASS()
class BATTLEBOTS_API APoisonSpell : public ASpellSystem
{
	GENERATED_BODY()
	
public:
  // Called after all components have been initialized with default values
  virtual void PostInitializeComponents() override;

  // How often does ignite deal damage
  UPROPERTY(EditDefaultsOnly, Category = "PoisonSpellConfig")
  float poisonTick;

  // How long to ignite the enemy player
  UPROPERTY(EditDefaultsOnly, Category = "PoisonSpellConfig")
  float poisonDuration;

  // Returns the damage event and type
  virtual FDamageEvent& GetDamageEvent() override;

protected:
  // Processes final elemental damage post item dmg modifiers
  virtual float ProcessElementalDmg(float initialDamage) override;

  virtual float GetPreProcessedDotDamage() override;

  // Deals damage to the actor and manages spell death. Override spell functionality, ex: Ignite, slow, etc.
  virtual void DealDamage(ABBotCharacter* enemyPlayer) override;

  // Process unique spell functionality such as turning poison skills into DOTs.
  virtual void DealUniqueSpellFunctionality(ABBotCharacter* enemyPlayer) override;

  // Returns poison dot duration
  virtual float GetFunctionalityDuration() override;

private:
  // Handles the ignite timers
  FTimerHandle poisonDotHandler;

  // Ignite delegate with an enemyPlayer payload
  FTimerDelegate poisonDotDelegate;

  // The damage done per poisonTick
  float poisonDotDamage;

  // The initial delay before poisoning the enemy player
  float poisonDotDelay;

  // Called on every poison tick
  UFUNCTION()
  void PoisonEnemy(ABBotCharacter* enemyPlayer);
};

