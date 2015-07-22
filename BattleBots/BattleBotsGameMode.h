// Copyright 2015 VMR Games, Inc. All Rights Reserved.
#pragma once
#include "Online/BBotsPlayerState.h"
#include "Online/BBotsBaseGameMode.h"
#include "GameFramework/GameMode.h"
#include "BattleBotsGameMode.generated.h"

// UCLASS(config=Game)

UCLASS(minimalapi)
class ABattleBotsGameMode : public ABBotsBaseGameMode
{
	GENERATED_BODY()

public:
	ABattleBotsGameMode(const FObjectInitializer& ObjectInitializer);
  
  // Starts the default timer to manage when the game should start
  virtual void PreInitializeComponents() override;

  /** always pick new random spawn */
  //virtual bool ShouldSpawnAtStartSpot(AController* Player) override;

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

  /** finish current match and lock players */
  UFUNCTION(exec)
  void FinishMatch();

protected:
  // Manages game timers for starting and ending the match.
  virtual void DefaultTimer();

  // Gets called when warm-up time is over and game is ready to start
  virtual void WarmUpTimeEnd();

  // If true the match will begin
  virtual bool ReadyToStartMatch();

  // The match ends when true
  virtual bool ReadyToEndMatch();

  // Handles the DefaultTimer timer - runs every second
  FTimerHandle defaultTimerHandler;
  FTimerHandle warmupTimerHandler;

  // The maximum number of rounds before exiting
  UPROPERTY(EditDefaultsOnly, Category = "Rules")
  int32 maxNumOfRounds;

  /** delay between first player login and starting match */
  UPROPERTY(EditDefaultsOnly, Category = "Rules")
  int32 warmupTime;

  /** match duration */
  UPROPERTY(EditDefaultsOnly, Category = "Rules")
  int32 roundTime;

  UPROPERTY(EditDefaultsOnly, Category = "Rules")
  int32 timeBetweenMatches;

  /** score for kill */
  UPROPERTY(EditDefaultsOnly, Category = "Score Rules")
  int32 killScore;

  /** score for death */
  UPROPERTY(EditDefaultsOnly, Category = "Score Rules")
  int32 deathScore;

  /* Can we deal damage to players in the same team */
  UPROPERTY(EditDefaultsOnly, Category = "Rules")
  bool bAllowFriendlyFireDamage;

  // Whether to respawn or spectate on death
  UPROPERTY(EditDefaultsOnly, Category = "Rules")
  bool bRespawnImmediately;

  // True when warmup is over
  bool bWarmUpTimerOver;

  /** check who won */
  virtual void DetermineMatchWinner();

  /** check if PlayerState is a winner */
  virtual bool IsWinner(class ABBotsPlayerState* PlayerState) const;

  /* Don't allow spectating of enemy teams */
  virtual bool CanSpectate(APlayerController* Viewer, APlayerState* ViewTarget);

private:
  // The time when the game started
  float gameStartTime;
};



