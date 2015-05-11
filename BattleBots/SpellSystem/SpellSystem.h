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
    bool isPiercing;
  UPROPERTY(EditDefaultsOnly, Category = "Config")
    bool canStun;
  UPROPERTY(EditDefaultsOnly, Category = "Config")
    float stunChance;
};

UCLASS()
class BATTLEBOTS_API ASpellSystem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpellSystem();

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

  // Returns the damage event and type
  UFUNCTION(BlueprintCallable, Category = "SpellSystem")
  virtual FDamageEvent& GetDamageEvent();

  UFUNCTION(BlueprintCallable, Category = "SpellSystem")
  float GetSpellCost() const;

  // Parents this spell to the caster actor
  virtual void SetCaster();

  // Sets the spell caster over the server
  UFUNCTION(Reliable, Server, WithValidation)
  void ServerSetCaster();
  virtual void ServerSetCaster_Implementation();
  virtual bool ServerSetCaster_Validate();

  /** A wrapper function that determines what type of spell to cast AOE, Instant...*/
  UFUNCTION(BlueprintCallable, Category = "SpellSystem")
  void CastSpell();

protected:
  FORCEINLINE virtual ABBotCharacter* GetSpellCaster() { return Caster; }

  /* Handle to manage the FX timer */
  FTimerHandle FXTimerHandle;

  // Casts the current spell
  virtual void CastSpell_Internal();

  // Processes final elemental damage post item dmg modifiers
  virtual float ProcessElementalDmg(float initialDamage);

  // Destroys spell after reaching a certain range or if it collides
  virtual void DestroySpell();

  // Simulate spell explosion
  virtual void SimulateExplosion();
private:
  // A reference to the caster of the spell
  UPROPERTY()
  ABBotCharacter* Caster;

  // A reference to the player controller
  ABattleBotsPlayerController* playerController;

  // Spell cooldown helper
  float CDHelper;

  // Holds the default dmg event and type
  FDamageEvent defaultDamageEvent;

  // Server side RPC for cast spell
  UFUNCTION(Reliable, Server, WithValidation)
  void ServerCastSpell();
  virtual void ServerCastSpell_Implementation();
  virtual bool ServerCastSpell_Validate();
};
