// // Copyright 2015 VMR Games, Inc. All Rights Reserved.

#include "BattleBots.h"
#include "IceSpell.h"


void AIceSpell::PostInitializeComponents()
{
  Super::PostInitializeComponents();

  if (HasAuthority())
  {
    // Set the default damage type to ice
    defaultDamageEvent.DamageTypeClass = UBBotDmgType_Ice::StaticClass();
  }
}


float AIceSpell::ProcessElementalDmg(float initialDamage)
{
  if (GetSpellCaster()) {
    float dmgMod = 1 + FMath::Clamp(GetSpellCaster()->GetDamageModifier_Ice(), -1.f, 1.f);
    return FMath::Abs(initialDamage * dmgMod);
  }
  else {
    GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Caster is null - Ice"));
    return initialDamage;
  }
}

FDamageEvent& AIceSpell::GetDamageEvent()
{
  return defaultDamageEvent;
}

void AIceSpell::DealUniqueSpellFunctionality(ABBotCharacter* enemyPlayer)
{
  if (HasAuthority())
  {
	  // Slow the enemy movement speed
	  SlowEnemy(enemyPlayer);
	
	  // Bind enemy player to SlowEnemyEnd delegate
	  slowMovementDelegate.BindUObject(this, &AIceSpell::SlowEnemyEnd, (ABBotCharacter*)enemyPlayer);
	  // Remove the slow effect after the slow duration
	  GetWorldTimerManager().SetTimer(slowMovementHandler, slowMovementDelegate, slowDuration, false);
  }
}

float AIceSpell::GetFunctionalityDuration()
{
  return slowDuration;
}

void AIceSpell::SlowEnemy(ABBotCharacter* enemyPlayer)
{
  if (HasAuthority())
  {
	  //TODO: might cause bugs with stance switches
	  enemyPlayer->SetMobilityModifier_All(MakeNegative(slowPercentage));
  }
}

void AIceSpell::SlowEnemyEnd(ABBotCharacter* enemyPlayer)
{
  if (HasAuthority())
  {
	  // Reverse the slow effect once the duration is up
	  enemyPlayer->SetMobilityModifier_All(FMath::Abs(slowPercentage));
	  // Clear the timer once the duration ends
	  GetWorldTimerManager().ClearTimer(slowMovementHandler);
  }
}
