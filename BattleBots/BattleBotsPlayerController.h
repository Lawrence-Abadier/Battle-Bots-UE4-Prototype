// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "AIController.h"
#include "Controllers/BBotsBasePC.h"
#include "BattleBotsGameMode.h"
#include "GameFramework/PlayerController.h"
#include "BattleBotsPlayerController.generated.h"


class ABBotCharacter;

UCLASS()
class ABattleBotsPlayerController : public ABBotsBasePC
{
	GENERATED_BODY()

public:
  /************************************************************************/
  /* Input and Controls                                                   */
  /************************************************************************/
	ABattleBotsPlayerController(const FObjectInitializer& ObjectInitializer);

protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

  // Called when the game starts or when spawned
  virtual void BeginPlay() override;

	// Begin PlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	// End PlayerController interface

  ABBotCharacter* ReferencePossessedPawn();
	/** Navigate player to the current mouse cursor location. */
	void MoveToMouseCursor();

	/** Navigate player to the current touch location. */
	void MoveToTouchLocation(const ETouchIndex::Type FingerIndex, const FVector Location);
	
	/** Navigate player to the given world location. */
	void SetNewMoveDestination(const FVector DestLocation);

	/** Input handlers for SetDestination action. */
	void OnSetDestinationPressed();
	void OnSetDestinationReleased();

	// Delegate called on right mouse click
	void CastOnRightClick();

  // Delegate called on hotbar slot press
  void HotBarSlot_One();
  void HotBarSlot_Two();
  void HotBarSlot_Three();
  void HotBarSlot_Four();

  // Stops updating local rotation when called
  void OnRotatationEnd();
	
  // Rotate player to mouse click location
	void RotateToMouseCursor();
  
  UFUNCTION(Reliable, Server, WithValidation)
  void ServerRotateToMouseCursor(FRotator newRotation);
  void ServerRotateToMouseCursor_Implementation(FRotator newRotation);
  bool ServerRotateToMouseCursor_Validate(FRotator newRotation);

private:
  // A reference to the possessed pawn
  UPROPERTY()
  ABBotCharacter* playerCharacter;
  
  // Used for replicating rotation on the client
  UPROPERTY(Replicated)
  FRotator localRotation;

  // Used for casting aoe spells. Limits spawning on the ground.
  UPROPERTY()
  TArray<TEnumAsByte<EObjectTypeQuery> > aoeObjTypes;

  // Used for character rotation.
  UPROPERTY()
  TArray<TEnumAsByte<EObjectTypeQuery> > rotObjTypes;

  // Rotation is only updated if true
  bool bRotChanged;

  // A simple debug function
	bool IsPossessedBy(ABBotCharacter* Character);

  // Helper function for casting spells on hotbar
  void CastFromSpellBarIndex(int32 index);

  // Get hit location under mouse click
  FVector GetMouseHitLocation(const TArray<TEnumAsByte<EObjectTypeQuery> >& ObjTypes);

  // A line trace under visibility channel
  FHitResult SingleLineTrace(const FVector& Start, const FVector& End);

  // Returns an impact point of the characters line of sight
  FVector GetLineOfSightImpactPoint();

  /************************************************************************/
  /* Respawning and Spectating                                            */
  /************************************************************************/
public:
  // Starts spectating on dead players
  void StartSpectating();

  // Returns time till spawn
  UFUNCTION(BlueprintCallable, Category = "Respawn")
  float GetTimeTillSpawn();

protected:
  /** update camera when pawn dies and enable spectating if applicable.*/
  virtual void PawnPendingDestroy(APawn* deadPawn) override;

  // Called after the respawn timer is up
  UFUNCTION()
  virtual void RespawnPlayer();

private:
  // The current GM in play
  ABattleBotsGameMode* currGM;

  // Whether to respawn or spectate on death
  bool bCanRespawn;

  // Respawns the player after respawn timer is up
  FTimerHandle RespawnHandler;

  // The initial respawn time
  float RespawnTime;

  // The respawn time scales per death as punishment
  float RespawnDeathScale;
};
