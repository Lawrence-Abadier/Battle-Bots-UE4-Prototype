// // Copyright 2015 VMR Games, Inc. All Rights Reserved.

#include "BattleBots.h"
#include "AOEPoisonSpell.h"




AAOEPoisonSpell::AAOEPoisonSpell()
  :Super()
{
  // Set to true to start destruction timer at instantiation.
  spellDataInfo.bIsPiercing = true;
}

void AAOEPoisonSpell::Tick(float DeltaSeconds)
{
  Super::Tick(DeltaSeconds);

  // Process aoe dmg per tick, and add an ignite dot to the player
  AOETick(DeltaSeconds);
}


void AAOEPoisonSpell::OnCollisionOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
  // Because the collision is managed in Tick, we need to override this method to prevent double collision processing
}

void AAOEPoisonSpell::DestroySpell()
{
  /* The spell gets automatically destroyed after spellDuration. */
}

void AAOEPoisonSpell::SimulateExplosion_Implementation()
{
  SetActorEnableCollision(false);
  SetActorHiddenInGame(true);
}

FVector AAOEPoisonSpell::GetSpellSpawnLocation()
{
  return spellSpawnLocation;
}

void AAOEPoisonSpell::DealDamage(ABBotCharacter* enemyPlayer)
{
  if (Role < ROLE_Authority)
  {
    // Deal damage only on the server
    ServerDealDamage(enemyPlayer);
  }
  else
  {
    UGameplayStatics::ApplyDamage(enemyPlayer, GetDamageToDeal(), GetInstigatorController(), this, GetDamageEvent().DamageTypeClass);
    // Apply damage while enemy is in the volume, then apply a poison dot.
    Super::DealDamage(enemyPlayer);
  }
}
