// Copyright 2015 VMR Games, Inc. All Rights Reserved.

#pragma once

#include "Character/BBotCharacter.h"
#include "GameFramework/Actor.h"
#include "SpellSystem.generated.h"

//class ABBotCharacter;

USTRUCT()
struct FSpellData{
  GENERATED_USTRUCT_BODY()

  UPROPERTY(EditDefaultsOnly, Category = "SpellInfo")
    FName spellName;
  UPROPERTY(EditDefaultsOnly, Category = "SpellInfo")
    FString toolTip;
  UPROPERTY(EditDefaultsOnly, Category = "SpellInfo")
    UTexture2D* icon;
  UPROPERTY(EditDefaultsOnly, Category = "Config")
    float spellDamage;
  UPROPERTY(EditDefaultsOnly, Category = "Config")
    float spellCost;
  UPROPERTY(EditDefaultsOnly, Category = "Config")
    float spellDuration;
  UPROPERTY(EditDefaultsOnly, Category = "Config")
    float spellSpeed;
  UPROPERTY(EditDefaultsOnly, Category = "Config")
    float coolDown;
  UPROPERTY(EditDefaultsOnly, Category = "Config")
    float castTime;
  UPROPERTY(EditDefaultsOnly, Category = "Config")
    bool bCastableWhileMoving;
  UPROPERTY(EditDefaultsOnly, Category = "Config")
    bool bIsPiercing;
  UPROPERTY(EditDefaultsOnly, Category = "Config")
    bool bCanStun;
  UPROPERTY(EditDefaultsOnly, Category = "Config")
    float stunChance;
  UPROPERTY(EditDefaultsOnly, Category = "Config")
    bool bKnockBack;
  UPROPERTY(EditDefaultsOnly, Category = "Config")
    float knockBackChance;
};

UCLASS()
class BATTLEBOTS_API ASpellSystem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpellSystem();

  // Called after all components have been initialized with default values
  virtual void PostInitializeComponents() override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

  /** An object that holds our spell configurations*/
  UPROPERTY(EditDefaultsOnly, Category = Config)
  FSpellData spellDataInfo;
  
  /** A simple collision primitive to use as the root component*/
  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Collision")
  USphereComponent* collisionComp;

  /** UStaticMeshComponent to represent the spell in the world.*/
  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Config")
  UStaticMeshComponent* spellMesh;

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PhysicsConfig")
  UProjectileMovementComponent* projectileMovementComp;

  UPROPERTY(VisibleDefaultsOnly)
  UParticleSystemComponent* particleComp;

  UPROPERTY(EditDefaultsOnly, Category = "Effects")
  UParticleSystem* spellFX;

  UPROPERTY(VisibleDefaultsOnly)
  UAudioComponent* audioComp;

  UPROPERTY(EditDefaultsOnly, Category = "Effects")
  USoundCue* explosionSound;

  // Is called when a spell collides with a player
  UFUNCTION()
  virtual void OnCollisionOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
  
  // Is called when a spell no longer collides with a player
  UFUNCTION()
  virtual void OnCollisionOverlapEnd(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
  
  // Returns the damage event and type
  UFUNCTION(BlueprintCallable, Category = "SpellSystem")
  virtual FDamageEvent& GetDamageEvent();

  UFUNCTION(BlueprintCallable, Category = "SpellSystem")
  float GetSpellCost() const;

  UFUNCTION(BlueprintCallable, Category = "SpellSystem")
  float GetCastTime() const;

  // Can the player cast the spell while moving?
  FORCEINLINE bool CastableWhileMoving() const { return spellDataInfo.bCastableWhileMoving; }

  /** Spawns a spell into the world. Manages internal spell cool down.*/
  UFUNCTION(BlueprintCallable, Category = "SpellSystem")
  void SpawnSpell(TSubclassOf<ASpellSystem> tempSpell);

protected:
  // Setting a member variable was delayed due to networked serialization, thus we have to cast a tempCaster so inherited classes can get the right spellCaster.
  // Returns the current spell's caster
  FORCEINLINE virtual ABBotCharacter* GetSpellCaster() { ABBotCharacter* tempCaster = Cast<ABBotCharacter>(GetInstigator()); return tempCaster; }

  // Returns the damage type of the spell
  FORCEINLINE TSubclassOf<UDamageType> ASpellSystem::GetDamageType()
  {
    return GetDamageEvent().DamageTypeClass;
  }
  
  //TODO: no need to replicate, it doesnt work in inherited classes
  // The final processed damage post damage modifiers
  float damageToDeal;

  // Holds the default dmg event and type
  FDamageEvent defaultDamageEvent;

  // Manages the spawned spell
  ASpellSystem* spellSpawner;

  // Casts the current spell
  virtual void SpawnSpell_Internal(TSubclassOf<ASpellSystem> tempSpell);

  // Processes final elemental damage post item dmg modifiers
  virtual float ProcessElementalDmg(float initialDamage);

  // Deals damage to the actor and manages spell death. Override spell functionality, ex: Ignite, slow, etc.
  virtual void DealDamage(ABBotCharacter* enemyPlayer);
  
  // Process unique spell functionality such as Ignite, Slow, Heal, Knockback, etc.
  virtual void DealUniqueSpellFunctionality(ABBotCharacter* enemyPlayer);

  /* Returns the duration the unique functionality duration, ex: Ignite Duration
  This prevents the spell object from getting deleted before the ignite duration is over */
  virtual float GetFunctionalityDuration();

  // Destroys spell after reaching a certain range or if it collides
  virtual void DestroySpell();

  // Simulate spell explosion
  virtual void SimulateExplosion();

private:

  // Holds all the overlapped actors to prevent multiple calls to dealdamage
  UPROPERTY()
  TArray<AActor*> OverlappedActors;

  // Spell cooldown helper
  float CDHelper;
  
  // Server side RPC for cast spell
  UFUNCTION(Reliable, Server, WithValidation)
  void ServerSpawnSpell(TSubclassOf<ASpellSystem> tempSpell);
  virtual void ServerSpawnSpell_Implementation(TSubclassOf<ASpellSystem> tempSpell);
  virtual bool ServerSpawnSpell_Validate(TSubclassOf<ASpellSystem> tempSpell);
};
