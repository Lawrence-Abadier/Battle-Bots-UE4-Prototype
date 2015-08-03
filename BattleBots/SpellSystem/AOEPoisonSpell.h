// // Copyright 2015 VMR Games, Inc. All Rights Reserved.

#pragma once

#include "SpellSystem/PoisonSpell.h"
#include "AOEPoisonSpell.generated.h"

/**
* AAOEPoisonSpell is the base class for a spell to be spawned in the level.
* AAOEPoisonSpell objects spawn at mouse hit location, leaving a volume behind
* that damages all enemies that stand in it. Enemies that stand in the volume
* also receive a poison dot for X durations. The duration of the poison gets reset
* to full duration as long as the enemy is within the volume.
*/
UCLASS()
class BATTLEBOTS_API AAOEPoisonSpell : public APoisonSpell
{
  GENERATED_BODY()

public:
  AAOEPoisonSpell();

  // Called after all components have been initialized with default values
  virtual void PostInitializeComponents() override;

  virtual void BeginPlay() override;

  // Is called when a spell collides with a player.
  virtual void OnCollisionOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

  // Returns the spawned spell at mouse location
  virtual FVector GetSpellSpawnLocation() override;

protected:
  // The rate the aoe ticks
  UPROPERTY(EditDefaultsOnly, Category = "AOE Config")
  float AoeTickInterval;

  virtual float GetPreProcessedDotDamage() override;

  // Deals damage to the actor and applies a poison dot.
  virtual void DealDamage(ABBotCharacter* enemyPlayer) override;

  /* The spell gets automatically destroyed after spellDuration.
  * We override this method to prevent spell destruction on contact.*/
  virtual void DestroySpell() override;

  /* Default AOESpells don't play a unique fx/sound at death,
  * instead uses an active fx/sound throughout the duration. */
  virtual void SimulateExplosion_Implementation() override;

private:
  // Enables AOE spells to tick
  FTimerHandle AOETickHandler;
};
