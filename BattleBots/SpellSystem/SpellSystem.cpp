// Copyright 2015 VMR Games, Inc. All Rights Reserved.

#include "BattleBots.h"
#include "Character/BBotCharacter.h"
#include "SpellSystem.h"


// Sets default values
ASpellSystem::ASpellSystem()
{
  // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = true;

  //Must be true for an Actor to replicate anything
  bReplicates = true;
  bReplicateMovement = true;
  bAlwaysRelevant = true;

  collisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisonComp"));
  collisionComp->OnComponentBeginOverlap.AddDynamic(this, &ASpellSystem::OnCollisionOverlapBegin);
  RootComponent = collisionComp;

  spellMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SpellMesh"));
  spellMesh->AttachParent = RootComponent;

  particleComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle Comp"));
  particleComp->AttachTo(RootComponent);
  particleComp->bAutoActivate = false;
  particleComp->bAutoDestroy = false;

  audioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComp"));
  audioComp->bAutoActivate = false;
  audioComp->bAutoDestroy = false;
  audioComp->AttachParent = RootComponent;

  projectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComp"));
  //projectileMovementComp->AttachParent = RootComponent;
  projectileMovementComp->InitialSpeed = spellDataInfo.spellSpeed;
  projectileMovementComp->ProjectileGravityScale = 0;
}

// Called when the game starts or when spawned
void ASpellSystem::BeginPlay()
{
  Super::BeginPlay();

  if (HasAuthority())
  {
    if (!GetSpellCaster()) {
      // Check if spell caster is set under server
      GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Yellow, TEXT("Spell caster is NULL"));
    }
  }
}

// Called every frame
void ASpellSystem::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);

}

// Called when a spell collides with a player
void ASpellSystem::OnCollisionOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
  ABBotCharacter* enemyPlayer = Cast<ABBotCharacter>(OtherActor);
  UStaticMeshComponent* staticWall = Cast<UStaticMeshComponent>(SweepResult.GetComponent());

  if (enemyPlayer && enemyPlayer != GetSpellCaster()) {
    // Check if our spell damaged the enemy already
    // 		if (!enemyPlayer->TookDamage()) {
    enemyPlayer->TakeDamage(ProcessElementalDmg(spellDataInfo.spellDamage), GetDamageEvent(), 0, this);
    // 		}
    // 		// Reset tookdamage and destroy the spell
    // 		enemyPlayer->SetTookDamage(false);
    //     
    //UGameplayStatics::ApplyDamage(enemyPlayer, ProcessElementalDmg(spellDataInfo.spellDamage), GetInstigatorController(), this, NULL);
    DestroySpell();
  }
  //     else if (staticWall)
  //     {
  //       GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Yellow, TEXT("Collided with the wall!!!"));
  //       spellDataInfo.isPiercing = false;
  //     }
  // Crashing editor
  else
  {
    GetWorldTimerManager().SetTimer(FXTimerHandle, this, &ASpellSystem::DestroySpell, spellDataInfo.spellDuration, false);
  }
  //}
}

float ASpellSystem::GetSpellCost() const
{
  return spellDataInfo.spellCost;
}

// No processing required for default damage types
float ASpellSystem::ProcessElementalDmg(float initialDamage)
{
    return initialDamage;
}

void ASpellSystem::CastSpell()
{
  // Check to see if our spell object is valid and not deleted by GC
  if (this->IsValidLowLevel()) {

    if (Role < ROLE_Authority) {
      ServerCastSpell();
    }
    else {
      float currentTime = GetWorld()->GetTimeSeconds();
      if (CDHelper < currentTime) {
        // Check if the cool down timer is up before casting
        CastSpell_Internal();
        CDHelper = currentTime + spellDataInfo.coolDown;
      }
    }
  }
}

void ASpellSystem::CastSpell_Internal()
{
  if (HasAuthority())
  {
    if (GetSpellCaster()) {
      FActorSpawnParameters spawnInfo;
      spawnInfo.Owner = GetOwner();
      spawnInfo.Instigator = Instigator;
      spawnInfo.bNoCollisionFail = true;
      GetWorld()->SpawnActor<ASpellSystem>(GetClass(),
                                           GetSpellCaster()->GetActorLocation(),
                                           GetSpellCaster()->GetActorRotation(),
                                           spawnInfo);
    }
  }
}

void ASpellSystem::ServerCastSpell_Implementation()
{
  CastSpell();
}

bool ASpellSystem::ServerCastSpell_Validate()
{
  return true;
}

FDamageEvent& ASpellSystem::GetDamageEvent()
{
  defaultDamageEvent.DamageTypeClass = UDamageType::StaticClass();
  return defaultDamageEvent;
}

// UDamageType* ASpellSystem::GetDamageType()
// {
//   return UDamageType::StaticClass();
// }

void ASpellSystem::DestroySpell()
{
  // If piercing, simulate explosion at spell death
  if (spellDataInfo.isPiercing) {
    this->SetLifeSpan(spellDataInfo.spellDuration);
    GetWorldTimerManager().SetTimer(FXTimerHandle, this, &ASpellSystem::SimulateExplosion, spellDataInfo.spellDuration, false);
  }
  else {
    // if it is not piercing then destroy spell at contact
    this->SetLifeSpan(0.1);
    this->SetActorEnableCollision(false);
    this->SetActorHiddenInGame(true);
    SimulateExplosion();
  }
}

void ASpellSystem::SimulateExplosion()
{
  // Play sound and particle effect on contact
  if (explosionSound) {
    UGameplayStatics::PlaySoundAtLocation(this, explosionSound, GetActorLocation());
  }
  if (spellFX) {
    UGameplayStatics::SpawnEmitterAtLocation(this, spellFX, GetActorLocation(), GetActorRotation());
  }
}





