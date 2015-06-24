// Copyright 2015 VMR Games, Inc. All Rights Reserved.
#pragma once
#include "Online/BBotsPlayerState.h"
#include "GameFramework/GameMode.h"
#include "BattleBotsGameMode.generated.h"

// UCLASS(config=Game)

UCLASS(minimalapi)
class ABattleBotsGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ABattleBotsGameMode(const FObjectInitializer& ObjectInitializer);
  
  /** always pick new random spawn */
  virtual bool ShouldSpawnAtStartSpot(AController* Player) override;

  /** notify about kills */
  virtual void Killed(AController* killer, AController* killedPlayer, APawn* killedPawn, const TSubclassOf<UDamageType> DamageType);

  /** can players damage each other? */
  virtual bool CanDealDamage(class ABBotsPlayerState* damageInstigator, class ABBotsPlayerState* damagedPlayer) const;

  virtual bool CanDealDamageTest(AController* damageInstigator, AController* damagedPlayer) const;

  /** called before startmatch */
  virtual void HandleMatchIsWaitingToStart() override;

  /** starts new match */
  virtual void HandleMatchHasStarted() override;

  // Returns true if players can respawn immediately
  UFUNCTION(BlueprintCallable, Category = "Game Rules")
  bool CanRespawnImmediately();

protected:

  /** delay between first player login and starting match */
//   UPROPERTY(config)
//   int32 warmupTime;

  /** match duration */
  UPROPERTY(config)
  int32 roundTime;

  UPROPERTY(config)
  int32 timeBetweenMatches;

  /** score for kill */
  UPROPERTY(config)
  int32 killScore;

  /** score for death */
  UPROPERTY(config)
  int32 deathScore;

  /** scale for self instigated damage */
  UPROPERTY(config)
  float damageSelfScale;

  /* Can we deal damage to players in the same team */
  UPROPERTY(EditDefaultsOnly, Category = "Rules")
  bool bAllowFriendlyFireDamage;

  // Whether to respawn or spectate on death
  UPROPERTY(EditDefaultsOnly, Category = "Rules")
  bool bRespawnImmediately;

  /** check who won */
  virtual void DetermineMatchWinner();

  /** check if PlayerState is a winner */
  virtual bool IsWinner(class ABBotsPlayerState* PlayerState) const;

  /* Don't allow spectating of enemy teams */
  virtual bool CanSpectate(APlayerController* Viewer, APlayerState* ViewTarget) override;
};



