// Copyright 2015 VMR Games, Inc. All Rights Reserved.

#pragma once

#include "BattleBotsCharacter.h"
#include "BattleBotsPlayerController.h"
#include "BBotCharacter.generated.h"

class ASpellSystem;

DECLARE_DELEGATE(FTimerDelegate)

UENUM(BlueprintType)
enum class EStanceType :uint8{
  EFrost        UMETA(DisplayName = "Frost"),
  EFire         UMETA(DisplayName = "Fire"),
  ELightning    UMETA(DisplayName = "Lightning"),
};

USTRUCT()
struct FCharacterAttributes{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float movementSpeed;
  UPROPERTY(EditDefaultsOnly, Category = "Defenses")
	float blockRate;
  UPROPERTY(EditDefaultsOnly, Category = "Defenses")
	float fireResist;
  UPROPERTY(EditDefaultsOnly, Category = "Defenses")
	float iceResist;
  UPROPERTY(EditDefaultsOnly, Category = "Defenses")
	float lightningResist;
  UPROPERTY(EditDefaultsOnly, Category = "Defenses")
  float holyResist;
  UPROPERTY(EditDefaultsOnly, Category = "Defenses")
	float poisonResist;
	UPROPERTY(EditDefaultsOnly, Category = "Defenses")
	float physicalResist;
  UPROPERTY(EditDefaultsOnly, Category = "BonusDamage")
  float bonusFireDmg;
  UPROPERTY(EditDefaultsOnly, Category = "BonusDamage")
  float bonusIceDmg;
  UPROPERTY(EditDefaultsOnly, Category = "BonusDamage")
  float bonusLightningDmg;
  UPROPERTY(EditDefaultsOnly, Category = "BonusDamage")
  float bonusHolyDmg;
  UPROPERTY(EditDefaultsOnly, Category = "BonusDamage")
  float bonusPoisonDmg;
  UPROPERTY(EditDefaultsOnly, Category = "BonusDamage")
  float bonusPhysicalDmg;
  UPROPERTY(EditDefaultsOnly, Category = "SpellCasting")
  float globalCooldown;
};

UCLASS(Blueprintable)
class BATTLEBOTS_API ABBotCharacter : public ABattleBotsCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
  ABBotCharacter(const FObjectInitializer& ObjectInitializer);
  
  // Called after all components have been initialized with default values
  virtual void PostInitializeComponents() override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

  /************************************************************************/
  /* Player Input and Collision                                           */
  /************************************************************************/
public:
  FORCEINLINE ABattleBotsPlayerController* GetBBOTController() const { return playerController; }
  FORCEINLINE ABattleBotsPlayerController* GetPC()
  {
    TObjectIterator<ABattleBotsPlayerController> Itr; //Not looping, just want the first entry
    if (!Itr) return nullptr;  //This can happen while PIE is exiting
    return *Itr;
  }

private:
  // A reference to the player controller
  ABattleBotsPlayerController* playerController;

  // Called to bind functionality to input
  virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

  // Called when our player collides with something
  UFUNCTION()
  void OnCollisionOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

  // Jump on SpaceBar Press and Release
  void OnJumpStart();
  void OnJumpEnd();

  // Called on mouse scroll up/down
  void OnScrollUp();
  void OnScrollDown();

  // Function called on right mouse click
  void CastOnRightClick();

  // Rotate character on mouseclick direction
  void RotateToMouseCursor();

  void ChangeFacingRotation(FRotator newRotation);

  UFUNCTION(Reliable, Server, WithValidation)
  void ServerChangeFacingRotation(FRotator newRotation);
  virtual void ServerChangeFacingRotation_Implementation(FRotator newRotation);
  virtual bool ServerChangeFacingRotation_Validate(FRotator newRotation);
  /************************************************************************/
  /* Character Attributes, Health, and Resource                           */
  /************************************************************************/
public:

  UFUNCTION(BlueprintCallable, Category = "PlayerCondition")
  float GetCurrentHealth() const;
  
  UFUNCTION(BlueprintCallable, Category = "PlayerCondition")
  float GetMaxHealth() const;
  
  UFUNCTION(BlueprintCallable, Category = "PlayerCondition")
  float GetCurrentOil() const;

  // Pass in positive number to increment, or a negative to decrement from current oil
  UFUNCTION(BlueprintCallable, Category = "PlayerCondition")
  void SetCurrentOil(float decOil);
  
  UFUNCTION(BlueprintCallable, Category = "PlayerCondition")
  float GetMaxOil() const;
  
  UFUNCTION(BlueprintCallable, Category = "PlayerCondition")
  bool IsAlive() const;

  // Sets the appropriate modifiers for DamageTypes speed. Ex: Bonus Fire Damage x%
  virtual void SetDamageModifier_All(float newDmgMod);

  // Sets the appropriate modifiers for walk/attack speed
  virtual void SetMobilityModifier_All(float newSpeedMod);

  // Sets the appropriate modifiers for block rate and resistances
  virtual void SetDefenseModifier_All(float newDefenseMod);

  // Sets the resist all by x%
  void SetResistAll(float newResistanceMod);

  // Returns the default class values
  FORCEINLINE FCharacterAttributes GetDefaultCharConfigValues() const { return GetClass()->GetDefaultObject<ABBotCharacter>()->characterConfig; }

  // Returns the bonus fire damage from items/buffs
  FORCEINLINE float GetDamageModifier_Fire() const { return FMath::Clamp(characterConfig.bonusFireDmg + spellBuffDebuffConfig.bonusFireDmg, -1.f, 1.f); }
  // Returns the bonus lightning damage from items/buffs
  FORCEINLINE float GetDamageModifier_Lightning() const { return FMath::Clamp(characterConfig.bonusLightningDmg + spellBuffDebuffConfig.bonusLightningDmg, -1.f, 1.f); }
  // Returns the bonus ice damage from items/buffs
  FORCEINLINE float GetDamageModifier_Ice() const { return FMath::Clamp(characterConfig.bonusIceDmg + spellBuffDebuffConfig.bonusIceDmg, -1.f, 1.f); }
  // Returns the bonus holy damage from items/buffs
  FORCEINLINE float GetDamageModifier_Holy() const { return FMath::Clamp(characterConfig.bonusHolyDmg + spellBuffDebuffConfig.bonusHolyDmg, -1.f, 1.f); }
  // Returns the bonus poison damage from items/buffs
  FORCEINLINE float GetDamageModifier_Poison() const { return FMath::Clamp(characterConfig.bonusPoisonDmg + spellBuffDebuffConfig.bonusPoisonDmg, -1.f, 1.f); }
  // Returns the bonus Physical damage from items/buffs
  FORCEINLINE float GetDamageModifier_Physical() const { return FMath::Clamp(characterConfig.bonusPhysicalDmg + spellBuffDebuffConfig.bonusPhysicalDmg, -1.f, 1.f); }

protected:
  // An object that holds the character configurations - Default Values + Stance Changes
  UPROPERTY(Replicated, EditDefaultsOnly, Category = "Config")
  FCharacterAttributes characterConfig;

  // Handles spell buffs and debuffs
  // Used for stance switches preventing certain edge cases.
  UPROPERTY(Replicated, EditDefaultsOnly, Category = "SpellBuffsDebuffs")
  FCharacterAttributes spellBuffDebuffConfig;

  // The character's health, variables within UStructs cannot be replicated
  UPROPERTY(EditDefaultsOnly, Transient, Category = "Attributes", Replicated)
  float health;
  // The character's main resource, used to cast spells
  UPROPERTY(EditDefaultsOnly, Transient, Category = "Attributes", Replicated)
  float oil;

  // The maximum value for health
  float maxHealth;

  // The maximum value for oil
  float maxOil;

  UFUNCTION(Reliable, Server, WithValidation)
  void ServerSetCurrentOil(float decOil);
  virtual void ServerSetCurrentOil_Implementation(float decOil);
  virtual bool ServerSetCurrentOil_Validate(float decOil);

  UFUNCTION(Reliable, Server, WithValidation)
    void ServerSetDamageModifier_All(float newDmgMod);
  virtual void ServerSetDamageModifier_All_Implementation(float newDmgMod);
  virtual bool ServerSetDamageModifier_All_Validate(float newDmgMod);

  UFUNCTION(Reliable, Server, WithValidation)
    void ServerSetMobilityModifier_All(float newSpeedMod);
  virtual void ServerSetMobilityModifier_All_Implementation(float newSpeedMod);
  virtual bool ServerSetMobilityModifier_All_Validate(float newSpeedMod);

  UFUNCTION(Reliable, Server, WithValidation)
    void ServerSetResistAll(float newResistanceMod);
  virtual void ServerSetResistAll_Implementation(float newResistanceMod);
  virtual bool ServerSetResistAll_Validate(float newResistanceMod);
  /************************************************************************/
  /* Damage and Death                                                     */
  /************************************************************************/
public:
  // Take damage and handle death
  virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
  
  // Checks if the player took damage to prevent multiple takedamage calls
  bool TookDamage() const;
  void SetTookDamage(bool bDamaged);
protected:
  // Must be overridden to handle block rating on appropriate classes
  virtual float ProcessDamageTypes(float Damage, struct FDamageEvent const& DamageEvent);

  // Process current negative/positive resistance on incoming damage type. 
  float ProcessFinalDmgPostResist(float initialDmg, float currentResist);

  UFUNCTION(Reliable, Server, WithValidation)
  void ServerSetTookDamage(bool bDamaged);
  virtual void ServerSetTookDamage_Implementation(bool bDamaged);
  virtual bool ServerSetTookDamage_Validate(bool bDamaged);
private:

  // True if the player took damage
  UPROPERTY(Replicated)
  bool bTookDamage;

  // Play animation, sound, and destroy pawn
  virtual void Die();

  /************************************************************************/
  /* SpellBar and Resource Management                                     */
  /************************************************************************/
public:
  // Coupled with UMG to manage character spells
  UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = "SpellBar")
  TArray<ASpellSystem*> spellBar;

  // Casts the spell at index
  UFUNCTION(BlueprintCallable, Category = "SpellBar")
  void CastFromSpellBar(int32 index);

  UFUNCTION(Reliable, Server, WithValidation)
  void ServerCastFromSpellBar(int32 index);
  virtual void ServerCastFromSpellBar_Implementation(int32 index);
  virtual bool ServerCastFromSpellBar_Validate(int32 index);

  // Adds a spell to our Spell Bar
  UFUNCTION(BlueprintCallable, Category = "SpellBar")
  void AddSpellToBar(TSubclassOf<ASpellSystem> newSpell);

  UFUNCTION(Reliable, Server, WithValidation)
  void ServerAddSpellToBar(TSubclassOf<ASpellSystem> newSpell);
  virtual void ServerAddSpellToBar_Implementation(TSubclassOf<ASpellSystem> newSpell);
  virtual bool ServerAddSpellToBar_Validate(TSubclassOf<ASpellSystem> newSpell);

  // Checks player resource before casting
  UFUNCTION(BlueprintCallable, Category = "SpellBar")
  bool CanCast(float spellCost) const;

protected:
  // Array of spell classes in Spell-Bar, Required by GetClass()
  UPROPERTY(Replicated)
  TArray<TSubclassOf<class ASpellSystem>> spellBar_Internal;

private:
  /* Handles the casting time of the spell
  *  Timer is cleared if the player moves */
  FTimerHandle castingSpellHandler;

  // Cast spell delegate with a spellBar index payload
  FTimerDelegate castingSpellDelegate;

  // Can the play cast the spell while moving?
  bool bCanCastWhileMoving;

  // Global cool down helper
  float GCDHelper;
  
  void CastFromSpellBar_Internal(int32 index);
  /************************************************************************/
  /* Character State                                                      */
  /************************************************************************/
public:
  // Getter and Setter for IsStunned
  bool GetIsStunned() const;
  UFUNCTION(BlueprintCallable, Category = "PlayerState")
  void SetIsStunned(bool stunned);

  // Knockback player
  void KnockbackPlayer(FVector spellPosition);

protected:
  // Disables character movement while true
  UPROPERTY(Transient, ReplicatedUsing = OnRep_IsStunned)
  bool bIsStunned;

  UFUNCTION(Reliable, Server, WithValidation)
  void ServerSetIsStunned(bool stunned);
  virtual void ServerSetIsStunned_Implementation(bool stunned);
  virtual bool ServerSetIsStunned_Validate(bool stunned);

  // Called when bIsStunned changes, Disables character movement while stunned
  UFUNCTION()
  virtual void OnRep_IsStunned();

  /************************************************************************/
  /* Combat Stances                                                       */
  /************************************************************************/
public:

  // Getter/Setter for the current stance.
  UFUNCTION(BlueprintCallable, Category = "CombatStance")
  EStanceType GetCurrentStance() const;
  UFUNCTION(BlueprintCallable, Category = "CombatStance")
  void SetCurrentStance(EStanceType newStance);

protected:
  // Limits the number of stance-switches per scroll up/down
  UPROPERTY(EditDefaultsOnly, Category = "CombatStance")
  float switchStanceCoolDown;

  // The 3 stances of the current archetype
  UPROPERTY(Replicated)
  TArray<EStanceType> combatStances;

  // The player's current combat stance
  UPROPERTY(EditDefaultsOnly, Category = "CombatStance", ReplicatedUsing = OnRep_StanceChanged)
  EStanceType currentStance;

  // Initialize the 3 class stances
  virtual void InitCombatStances();

  // Switch stances in a round robin fashion
  UFUNCTION(BlueprintCallable, Category = "CombatStance")
  void SwitchCombatStance();

  // Called when current stance changes, to process stance related buffs
  UFUNCTION()
  virtual void OnRep_StanceChanged();

  // OnRep does not run automatically on the server for C++
  UFUNCTION(Reliable, Server, WithValidation)
    void ServerOnRep_StanceChanged();
  virtual void ServerOnRep_StanceChanged_Implementation();
  virtual bool ServerOnRep_StanceChanged_Validate();

  // Can only change current stance on the server
  UFUNCTION(Reliable, Server, WithValidation)
  void ServerSetCurrentStance(EStanceType newStance);
  virtual void ServerSetCurrentStance_Implementation(EStanceType newStance);
  virtual bool ServerSetCurrentStance_Validate(EStanceType newStance);

  virtual void SetToMobilityStance();
  virtual void SetToDamageStance();
  virtual void SetToDefenseStance();

  // Prints debug msg of the current stance
  virtual void printCurrentStance();

private:

  // The current stance array index
  int32 stanceIndex;

  // Assists in implementing an SSCD
  float switchStanceCDHelper;

  // True if the player scrolls up
  bool bScrolledUp;

  // Implements SS CD on the server
  void SwitchCombatStanceHelper(bool bScrolled);

  // Can only switch current stance on the server
  UFUNCTION(Reliable, Server, WithValidation)
    void ServerSwitchCombatStanceHelper(bool bScrolled);
  virtual void ServerSwitchCombatStanceHelper_Implementation(bool bScrolled);
  virtual bool ServerSwitchCombatStanceHelper_Validate(bool bScrolled);
};
