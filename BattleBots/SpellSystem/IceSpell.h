// // Copyright 2015 VMR Games, Inc. All Rights Reserved.

#pragma once

#include "SpellSystem/DamageTypes/BBotDmgType_Ice.h"
#include "SpellSystem/SpellSystem.h"
#include "IceSpell.generated.h"


/**
 * AIceSpell is the base class for a spell to be spawned in the level.
 * AIceSpell objects are projectiles that spawn at spell caster location.
 * The projectiles get destroyed on contact. Enemies hit by the spell
 * receive a slow for X durations. The duration of the slow gets reset
 * to full duration if the enemy is hit before the duration ends.
 */
UCLASS()
class BATTLEBOTS_API AIceSpell : public ASpellSystem
{
	GENERATED_BODY()
	
public:
  // Called after all components have been initialized with default values
  virtual void PostInitializeComponents() override;

  // Returns the damage event and type
  virtual FDamageEvent& GetDamageEvent() override;

  // Clears the slow timers
  virtual void ClearUniqueTimers() override;

  // TODO: Create a custom math library
  // Returns the negative version of num 
  template<class T>
  static FORCEINLINE T MakeNegative(const T num) { return FMath::Abs(num) * -1; }

protected:
  // Processes final elemental damage post item dmg modifiers
  virtual float ProcessElementalDmg(float initialDamage) override;

  // The enemy player is slowed by x%
  UPROPERTY(EditDefaultsOnly, Category = "IceSpellConfig")
  float slowPercentage;

  // How long to slow the enemy player
  UPROPERTY(EditDefaultsOnly, Category = "IceSpellConfig")
  float slowDuration;

  /*  virtual UDamageType* GetDamageType() override;*/

  // Process unique spell functionality such as slow enemy movement.
  virtual void DealUniqueSpellFunctionality(ABBotCharacter* enemyPlayer) override;

  // Returns the slow duration
  virtual float GetFunctionalityDuration() override;

private:
  // The time at the start of the slow
  float currentTime;

  // Handles the slow player timers
  FTimerHandle slowMovementHandler;

  // Ignite delegate with an enemyPlayer payload
  FTimerDelegate slowMovementDelegate;

  // Slows the enemy movement speed
  UFUNCTION()
  void SlowEnemy(ABBotCharacter* enemyPlayer);

  // Removes the slow effect and clears the slow timer
  UFUNCTION()
  void SlowEnemyEnd(ABBotCharacter* enemyPlayer);
};