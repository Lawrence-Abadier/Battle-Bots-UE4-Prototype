// // Copyright 2015 VMR Games, Inc. All Rights Reserved.

#include "BattleBots.h"
#include "PoisonSpell.h"



void APoisonSpell::PostInitializeComponents()
{
  Super::PostInitializeComponents();

  if (HasAuthority())
  {
    // Set the damage type of the spell to poison
    defaultDamageEvent.DamageTypeClass = UBBotDmgType_Poison::StaticClass();
    // Set the dot damage per poison tick
    poisonDotDamage = ProcessElementalDmg(spellDataInfo.spellDamage) / poisonDuration;
    // Acts as an offset to prevent edge cases with odd tick durations
    poisonDotDelay = poisonTick / 2;
  }
}

float APoisonSpell::ProcessElementalDmg(float initialDamage)
{
  if (GetSpellCaster()) {
    float dmgMod = 1 + FMath::Clamp(GetSpellCaster()->GetDamageModifier_Poison(), -1.f, 1.f);    
    return FMath::Abs(initialDamage * dmgMod);
  }
  else {
    GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Caster is null - poison"));
    return initialDamage;
  }
}

FDamageEvent& APoisonSpell::GetDamageEvent()
{
  return defaultDamageEvent;
}

void APoisonSpell::DealDamage(ABBotCharacter* enemyPlayer)
{
  // Deal poison damage per poisonTick on the enemy player
  DealUniqueSpellFunctionality(enemyPlayer);

  DestroySpell();
}

void APoisonSpell::DealUniqueSpellFunctionality(ABBotCharacter* enemyPlayer)
{
  poisonDotDelegate.BindUObject(this, &APoisonSpell::PoisonEnemy, (ABBotCharacter*)enemyPlayer);

  poisonDuration = GetFunctionalityDuration() + GetWorld()->GetTimeSeconds();
  GetWorldTimerManager().SetTimer(poisonDotHandler, poisonDotDelegate, poisonTick, true, poisonDotDelay);
}

void APoisonSpell::PoisonEnemy(ABBotCharacter* enemyPlayer)
{
  if (poisonDuration <= GetWorld()->GetTimeSeconds())
  {
    GetWorldTimerManager().ClearTimer(poisonDotHandler);
  }
  else
  {
    UGameplayStatics::ApplyDamage(enemyPlayer, poisonDotDamage, GetInstigatorController(), this, GetDamageType());
  }
}

float APoisonSpell::GetFunctionalityDuration()
{
  return GetClass()->GetDefaultObject<APoisonSpell>()->poisonDuration;
}