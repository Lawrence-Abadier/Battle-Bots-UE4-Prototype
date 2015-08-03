// // Copyright 2015 VMR Games, Inc. All Rights Reserved.

#include "BattleBots.h"
#include "AOEFireSpell.h"


AAOEFireSpell::AAOEFireSpell()
  :Super()
{
  // Set to true to start destruction timer at instantiation.
  spellDataInfo.bIsPiercing = true;

  AoeTickInterval = 0.2;

  collisionComp->InitSphereRadius(200.f);
}


void AAOEFireSpell::PostInitializeComponents()
{
  Super::PostInitializeComponents();

  if (HasAuthority())
  {
    // Sets the dmg done per tick
    SetDamageToDeal(ProcessElementalDmg(GetPreProcessedDotDamage()));
  }
}

void AAOEFireSpell::BeginPlay()
{
  Super::BeginPlay();

  if (HasAuthority())
  {
    // Enables AOE Tick on spawn
    GetWorldTimerManager().SetTimer(AOETickHandler, this, &AAOEFireSpell::AOETick, AoeTickInterval, true);
  }
}


void AAOEFireSpell::OnCollisionOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
  // Because the damage is managed in AOETick, we need to override this method to prevent double processing
  ABBotCharacter* enemyPlayer = Cast<ABBotCharacter>(OtherActor);

  if (IsEnemy(enemyPlayer)) {
    if (!OverlappedActors.Contains(enemyPlayer))
    {
      OverlappedActors.AddUnique(enemyPlayer);
    }
  }
}

void AAOEFireSpell::DestroySpell()
{
  /* The spell gets automatically destroyed after spellDuration. */
  GetWorldTimerManager().ClearTimer(AOETickHandler);
}

void AAOEFireSpell::SimulateExplosion_Implementation()
{
  SetActorEnableCollision(false);
  SetActorHiddenInGame(true);
}

FVector AAOEFireSpell::GetSpellSpawnLocation()
{
  return spellSpawnLocation;
}

void AAOEFireSpell::DealDamage(ABBotCharacter* enemyPlayer)
{
  if (Role < ROLE_Authority)
  {
    // Deal damage only on the server
    ServerDealDamage(enemyPlayer);
  }
  else
  {
    UGameplayStatics::ApplyDamage(enemyPlayer, GetDamageToDeal(), GetInstigatorController(), this, GetDamageEvent().DamageTypeClass);

    // Ignite on crit?
    DealUniqueSpellFunctionality(enemyPlayer);
  }
}

float AAOEFireSpell::GetPreProcessedDotDamage()
{
  return damagePerSecond * AoeTickInterval;
}

