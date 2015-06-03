// // Copyright 2015 VMR Games, Inc. All Rights Reserved.

#pragma once

#include "SpellSystem/DamageTypes/BBotDmgType_Fire.h"
#include "SpellSystem/SpellSystem.h"
#include "FireSpell.generated.h"

DECLARE_DELEGATE(FTimerDelegate)

/**
 * 
 */
UCLASS()
class BATTLEBOTS_API AFireSpell : public ASpellSystem
{
  GENERATED_BODY()

public:
  // Called after all components have been initialized with default values
  virtual void PostInitializeComponents() override;

  // The enemy player takes x% of total damage every ignite tick
  UPROPERTY(EditDefaultsOnly, Category = "FireSpellConfig")
  float ignitePercentage;

  // How often does ignite deal damage
  UPROPERTY(EditDefaultsOnly, Category = "FireSpellConfig")
  float igniteTick;

  // How long to ignite the enemy player
  UPROPERTY(EditDefaultsOnly, Category = "FireSpellConfig")
  float igniteDuration;

  // Returns the damage event and type
  virtual FDamageEvent& GetDamageEvent() override;

protected:
  // Process unique spell functionality such as Ignite.
  virtual void DealUniqueSpellFunctionality(ABBotCharacter* enemyPlayer) override;

  virtual float GetFunctionalityDuration() override;

  // Processes final elemental damage post item dmg modifiers
  virtual float ProcessElementalDmg(float initialDamage) override;

private:
  // Handles the ignite timers
  FTimerHandle igniteHandler;
  
  // Ignite delegate with an enemyPlayer payload
  FTimerDelegate igniteDelegate;

  // The damage done per igniteTick
  UPROPERTY()
  float igniteDamage;

  // The initial delay for the first ignite
  float igniteDelay;

  UFUNCTION()
  void IgniteEnemy(ABBotCharacter* enemyPlayer);
};
