// // Copyright 2015 VMR Games, Inc. All Rights Reserved.

#include "BattleBots.h"
#include "AOEIceSpell.h"




AAOEIceSpell::AAOEIceSpell()
  :Super()
{
  // Set to true to start destruction timer at instantiation.
  spellDataInfo.bIsPiercing = true;

  AoeTickInterval = 0.2;

  collisionComp->InitSphereRadius(200.f);
  collisionComp->SetCollisionProfileName(FName(TEXT("OverlapAll")));
}

void AAOEIceSpell::PostInitializeComponents()
{
  Super::PostInitializeComponents();

  if (HasAuthority())
  {
    // Sets the dmg done per tick
    SetDamageToDeal(ProcessElementalDmg(GetPreProcessedDotDamage()));
  }
}

void AAOEIceSpell::BeginPlay()
{
  Super::BeginPlay();

  if (HasAuthority())
  {
    // Enables AOE Tick on spawn
    GetWorldTimerManager().SetTimer(AOETickHandler, this, &AAOEIceSpell::AOETick, AoeTickInterval, true);
  }
}

void AAOEIceSpell::OnCollisionOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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

void AAOEIceSpell::DestroySpell()
{
  /* The spell gets automatically destroyed after spellDuration. */
  GetWorldTimerManager().ClearTimer(AOETickHandler);
}

void AAOEIceSpell::SimulateExplosion_Implementation()
{
  SetActorEnableCollision(false);
  SetActorHiddenInGame(true);
}

FVector AAOEIceSpell::GetSpellSpawnLocation()
{
  return spellSpawnLocation;
}

float AAOEIceSpell::GetPreProcessedDotDamage()
{
  return damagePerSecond * AoeTickInterval;
}

void AAOEIceSpell::DealDamage(ABBotCharacter* enemyPlayer)
{
  if (HasAuthority())
  {
    // Deal damage only on the server

    UGameplayStatics::ApplyDamage(enemyPlayer, GetDamageToDeal(), GetInstigatorController(), this, GetDamageEvent().DamageTypeClass);

    DealUniqueSpellFunctionality(enemyPlayer);
  }
}
