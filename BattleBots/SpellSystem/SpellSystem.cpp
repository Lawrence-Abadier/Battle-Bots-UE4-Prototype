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
}

// Called when the game starts or when spawned
void ASpellSystem::BeginPlay()
{
  Super::BeginPlay();

  // Sets caster of the spell
  if (!Caster) {
    SetCaster();
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
  if (enemyPlayer && enemyPlayer != GetSpellCaster()) {
    // Check if our spell damaged the enemy already
    if (!enemyPlayer->TookDamage()) {
      enemyPlayer->TakeDamage(ProcessElementalDmg(spellDataInfo.spellDamage), GetDamageEvent(), 0, this);
    }
    // Reset tookdamage and destroy the spell
    //this->SetLifeSpan(0.1);
    enemyPlayer->SetTookDamage(false);
    // Crashing editor
    DestroySpell();
  }
}

float ASpellSystem::GetSpellCost() const
{
  return spellDataInfo.spellCost;
}

void ASpellSystem::SetCaster()
{
//   if (Role < ROLE_Authority) {
//     ServerSetCaster();
//   }
//   else {
    APlayerController* pc = NULL;
    for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator) {
      pc = Iterator->Get();

      if (pc->IsLocalPlayerController()) {
        playerController = Cast<ABattleBotsPlayerController>(pc);
      }
    }
    Caster = Cast<ABBotCharacter>(playerController->GetPawn());
/*  }*/
}

void ASpellSystem::ServerSetCaster_Implementation()
{
  SetCaster();
}

bool ASpellSystem::ServerSetCaster_Validate()
{
  return true;
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
      if (CDHelper < currentTime)
      {
        // Check if the cool down timer is up before casting
        CastSpell_Internal();
        CDHelper = currentTime + spellDataInfo.coolDown;
      }
    }
  }
}

void ASpellSystem::CastSpell_Internal()
{
  FActorSpawnParameters spawnInfo;
  spawnInfo.Owner = GetSpellCaster();
  spawnInfo.Instigator = Instigator;
  spawnInfo.bNoCollisionFail = true;
  GetWorld()->SpawnActor<ASpellSystem>(GetClass(),
                                       GetSpellCaster()->GetActorLocation(),
                                       GetSpellCaster()->GetActorRotation(),
                                       spawnInfo);
}

void ASpellSystem::ServerCastSpell_Implementation()
{
  CastSpell();
}

bool ASpellSystem::ServerCastSpell_Validate()
{
  return true;
}

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
    //this->SetActorHiddenInGame(true);
    SimulateExplosion();
  }
}

void ASpellSystem::SimulateExplosion()
{
  // Play sound and particle effect on contact
  if (explosionSound) {
    audioComp->SetSound(explosionSound);
    audioComp->Play();
  }
  if (particleComp) {
    particleComp->SetTemplate(spellFX);
    particleComp->ActivateSystem();
  }
}

FDamageEvent& ASpellSystem::GetDamageEvent()
{
  defaultDamageEvent.DamageTypeClass = UDamageType::StaticClass();
  return defaultDamageEvent;
}





