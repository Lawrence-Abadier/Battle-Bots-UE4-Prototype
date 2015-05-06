// Copyright 2015 VMR Games, Inc. All Rights Reserved.

#pragma once

#include "SpellSystem/SpellSystem.h"
#include "BattleBotsCharacter.h"
#include "BattleBotsPlayerController.h"
#include "BBotCharacter.generated.h"

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
	float frostResist;
  UPROPERTY(EditDefaultsOnly, Category = "Defenses")
	float lightningResist;
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
};

UCLASS(Blueprintable)
class BATTLEBOTS_API ABBotCharacter : public ABattleBotsCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
  ABBotCharacter(const FObjectInitializer& ObjectInitializer);
  
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

  /************************************************************************/
  /* Player Input and Collision                                           */
  /************************************************************************/
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

  // Returns the bonus fire damage from items/buffs
  FORCEINLINE float GetDamageModifier_Fire() const { return characterConfig.bonusFireDmg; }
  // Returns the bonus lightning damage from items/buffs
  FORCEINLINE float GetDamageModifier_Lightning() const { return characterConfig.bonusLightningDmg; }
  // Returns the bonus ice damage from items/buffs
  FORCEINLINE float GetDamageModifier_Ice() const { return characterConfig.bonusIceDmg; }
  // Returns the bonus holy damage from items/buffs
  FORCEINLINE float GetDamageModifier_Holy() const { return characterConfig.bonusHolyDmg; }
  // Returns the bonus poison damage from items/buffs
  FORCEINLINE float GetDamageModifier_Poison() const { return characterConfig.bonusPoisonDmg; }
  // Returns the bonus Physical damage from items/buffs
  FORCEINLINE float GetDamageModifier_Physical() const { return characterConfig.bonusPhysicalDmg; }

protected:
  // An object that holds the character configurations
  UPROPERTY(EditDefaultsOnly, Category = Config)
  FCharacterAttributes characterConfig;

  // The character's health, variables within UStructs cannot be replicated
  UPROPERTY(EditDefaultsOnly, Category = "Attributes", Replicated)
  float health;
  // The character's main resource, used to cast spells
  UPROPERTY(EditDefaultsOnly, Category = "Attributes", Replicated)
  float oil;

  // The maximum value for health
  float maxHealth;

  // The maximum value for oil
  float maxOil;

  UFUNCTION(Reliable, Server, WithValidation)
  void ServerSetCurrentOil(float decOil);
  virtual void ServerSetCurrentOil_Implementation(float decOil);
  virtual bool ServerSetCurrentOil_Validate(float decOil);

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
  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SpellBar")
  TArray<ASpellSystem*> spellBar;
  
  // Casts the spell at index
  UFUNCTION(BlueprintCallable, Category = "SpellBar")
  void CastFromSpellBar(int32 index);

  // Adds a spell to our Spell Bar
  UFUNCTION(BlueprintCallable, Category = "SpellBar")
  void AddSpellToBar(TSubclassOf<ASpellSystem> newSpell);

  // Checks player resource before casting
  UFUNCTION(BlueprintCallable, Category = "SpellBar")
  bool CanCast(float spellCost) const;

private:
  // Array of spell classes in Spell-Bar, Required by GetClass()
  UPROPERTY()
  TArray<TSubclassOf<class ASpellSystem>> spellBar_Internal;

  // Helper function for CastFromSpellBar
  UFUNCTION()
  void CastFromSpellBar_Internal(int32 index);
  
  /************************************************************************/
  /* Character State                                                      */
  /************************************************************************/
public:
  // Getter and Setter for IsStunned
  bool GetIsStunned() const;
  void SetIsStunned(bool stunned);

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
  UPROPERTY()
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

  // Can only change current stance on the server
  UFUNCTION(Reliable, Server, WithValidation)
  void ServerSetCurrentStance(EStanceType newStance);
  virtual void ServerSetCurrentStance_Implementation(EStanceType newStance);
  virtual bool ServerSetCurrentStance_Validate(EStanceType newStance);

  // Inits the stances on the server
  UFUNCTION(Reliable, Server, WithValidation)
  void ServerInitCombatStances();
  virtual void ServerInitCombatStances_Implementation();
  virtual bool ServerInitCombatStances_Validate();

  // Prints debug msg of the current stance
  virtual void printCurrentStance();

private:

  // The current stance array index
  int32 stanceIndex;

  // Assists in implementing an SSCD
  float switchStanceCDHelper;

  // True if the player scrolls up
  bool bScrolledUp;

  void SwitchCombatStanceHelper(bool bScrolled);
};
