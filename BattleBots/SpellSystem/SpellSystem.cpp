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

  spellFX = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle Comp"));
  spellFX->AttachTo(RootComponent);

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
  if (enemyPlayer && enemyPlayer != Caster) {
    // Check if our spell damaged the enemy already
    if (!enemyPlayer->TookDamage()) {
      enemyPlayer->TakeDamage(10, FDamageEvent(), 0, this);
    }
    // Calculate event aoe or any damage
    //InitDamageEvent();
    // process type damage based on char attr, ex FireSpells do 20% more damage
    // pass the event and processed damage to take damage.
    //GetInstigator();

    // Reset tookdamage and destroy the spell
    //this->SetLifeSpan(0.1);
    enemyPlayer->SetTookDamage(false);
    // Crashing editor
    //DestroySpell();
  }
}

float ASpellSystem::GetSpellCost() const
{
  return spellDataInfo.spellCost;
}

void ASpellSystem::SetCaster(ABBotCharacter* caster)
{
  Caster = caster;
  //AttachRootComponentTo(caster->GetRootComponent());
}

void ASpellSystem::CastSpell()
{
  FActorSpawnParameters spawnInfo;
  spawnInfo.Owner = Caster;
  spawnInfo.Instigator = Instigator;
  spawnInfo.bNoCollisionFail = true;
  GetWorld()->SpawnActor<ASpellSystem>(GetClass(),
                                       Caster->GetActorLocation(),
                                       Caster->GetActorRotation(),
                                       spawnInfo);

//  ASpellSystem *spell = GetWorld()->SpawnActor<ASpellSystem>(BpSpell, Caster->GetActorLocation() + 10, Caster->GetActorRotation(), spawnInfo);
//   // Check to see if our spell object is valid and not deleted by GC
//   if (this->IsValidLowLevel()) {
//      
//     if (Role < ROLE_Authority) {
//       ServerCastSpell();
//     }
//     else {
//       ASpellSystem* const ASpell = GetWorld()->SpawnActor<ASpellSystem>(this->StaticClass(), Caster->GetActorForwardVector(), Caster->GetActorRotation());
//     }
//   }
}

void ASpellSystem::ServerCastSpell_Implementation()
{
  //CastSpell();
}

bool ASpellSystem::ServerCastSpell_Validate()
{
  return true;
}

void ASpellSystem::DestroySpell()
{
  if (spellDataInfo.isPiercing) {
    //FTimerHandle MyHandle;
    //GetWorldTimerManager().SetTimer(MyHandle, )

    this->SetActorEnableCollision(false);
    this->SetActorHiddenInGame(true);
    this->SetLifeSpan(spellDataInfo.spellDuration);
  }
  else {
    // if it is not piercing then destroy spell at contact
    this->SetLifeSpan(0.1);
  }
}




