// // Copyright 2015 VMR Games, Inc. All Rights Reserved.

#include "BattleBots.h"
#include "AOEFireSpell.h"


AAOEFireSpell::AAOEFireSpell()
  :Super()
{
  // Set to true to start destruction timer at instantiation.
  spellDataInfo.bIsPiercing = true;
}

void AAOEFireSpell::Tick(float DeltaSeconds)
{
  Super::Tick(DeltaSeconds);

  // Process aoe dmg per tick, and add an ignite dot to the player
  AOETick(DeltaSeconds);
}


void AAOEFireSpell::OnCollisionOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
  // Because the collision is managed in Tick, we need to override this method to prevent double collision processing
}

void AAOEFireSpell::DestroySpell()
{
  /* The spell gets automatically destroyed after spellDuration. */
}

void AAOEFireSpell::SimulateExplosion()
{
  SetActorEnableCollision(false);
  SetActorHiddenInGame(true);
}

FVector AAOEFireSpell::GetSpellSpawnLocation()
{
  return spellSpawnLocation;
}





