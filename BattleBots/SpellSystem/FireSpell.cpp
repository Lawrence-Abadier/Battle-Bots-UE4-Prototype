// // Copyright 2015 VMR Games, Inc. All Rights Reserved.

#include "BattleBots.h"
#include "FireSpell.h"


void AFireSpell::PostInitializeComponents()
{
  Super::PostInitializeComponents();

  if (HasAuthority())
  {
    // Set damage type
    defaultDamageEvent.DamageTypeClass = UBBotDmgType_Fire::StaticClass();
    // Set the ignite damage per ignite tick
    igniteDamage = FMath::Clamp(ignitePercentage, 0.f, 1.f) * ProcessElementalDmg(GetPreProcessedDotDamage());
    // Acts as an offset to prevent edge cases with odd tick durations
    igniteDelay = igniteTick / 2;
  }
}

float AFireSpell::ProcessElementalDmg(float initialDamage)
{
  if (GetSpellCaster())
  {
    float dmgMod = 1 + FMath::Clamp(GetSpellCaster()->GetDamageModifier_Fire(), -1.f, 1.f);
    GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Yellow, FString::FromInt(100 * dmgMod));
    return FMath::Abs(initialDamage * dmgMod);
  }
  else {
    GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Fire Caster Is null!!"));
    return initialDamage;
  }
}

float AFireSpell::GetPreProcessedDotDamage()
{
  return spellDataInfo.spellDamage / igniteDuration;
}


FDamageEvent& AFireSpell::GetDamageEvent()
{
  return defaultDamageEvent;
}

// Adds an ignite dot on the player
void AFireSpell::DealUniqueSpellFunctionality(ABBotCharacter* enemyPlayer)
{
  if (HasAuthority())
  {
	  igniteDelegate.BindUObject(this, &AFireSpell::IgniteEnemy, (ABBotCharacter*)enemyPlayer);
	
	  igniteDuration = GetFunctionalityDuration() + GetWorld()->GetTimeSeconds();
	  GetWorldTimerManager().SetTimer(igniteHandler, igniteDelegate, igniteTick, true, igniteDelay);
  }
}

void AFireSpell::IgniteEnemy(ABBotCharacter* enemyPlayer)
{
  if (HasAuthority())
  {
	  if (igniteDuration <= GetWorld()->GetTimeSeconds())
	  {
	    GetWorldTimerManager().ClearTimer(igniteHandler);
	  }
    else if (enemyPlayer)
	  {
	    UGameplayStatics::ApplyDamage(enemyPlayer, igniteDamage, GetInstigatorController(), this, GetDamageType());
	  }
  }
}

float AFireSpell::GetFunctionalityDuration()
{
  return GetClass()->GetDefaultObject<AFireSpell>()->igniteDuration;
}