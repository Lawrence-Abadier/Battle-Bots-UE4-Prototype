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
  virtual bool CanDealDamage(AController* damageInstigator, AController* damagedPlayer) const;

  /** called before startmatch */
  virtual void HandleMatchIsWaitingToStart() override;

  /** starts new match */
  virtual void HandleMatchHasStarted() override;

  // Returns true if players can respawn immediately
  UFUNCTION(BlueprintCallable, Category = "Game Rules")
  bool CanRespawnImmediately();

  FORCEINLINE float GetRespawnTime(){ return RespawnTime; }
  FORCEINLINE float GetRespawnDeathScale(){ return RespawnDeathScale; }

  /** finish current match and lock players */
  UFUNCTION(exec)
  void FinishMatch();

protected:

  // Initializes the gamestate with the warm-up time
  virtual void InitWarmupRound();

  // Manages game timers for starting and ending the match.
  virtual void DefaultTimer();

  // Gets called when warm-up time is over and game is ready to start
  virtual void WarmUpTimeEnd();

  // Resets game to initial state
  virtual void EndOfRoundReset();

  // If true the match will begin
  virtual bool ReadyToStartMatch();

  // The match ends when true
  virtual bool ReadyToEndMatch();

  virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;


  /* Returns a respawn spot based on team number.
  This is required so if mid match we swap team locations,
  the players would spawn in the correct updated location
  and not the original spawn spot at the beginning of the game. */
  virtual AActor* FindPlayerStart_Implementation(AController* Player, const FString& IncomingName) override;

  // Handles the DefaultTimer timer - runs every second
  FTimerHandle defaultTimerHandler;
  FTimerHandle warmupTimerHandler;

  // The maximum number of rounds before exiting
  UPROPERTY(EditDefaultsOnly, Category = "Rules")
  int32 maxNumOfRounds;

  /** delay between first player login and starting match */
  UPROPERTY(EditDefaultsOnly, Category = "Rules")
  float warmupTime;

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

  // The initial respawn time
  UPROPERTY(EditDefaultsOnly, Category = "Respawn")
  float RespawnTime;
  
  // The respawn time scales per death as punishment
  UPROPERTY(EditDefaultsOnly, Category = "Respawn")
  float RespawnDeathScale;

  // Whether to respawn or spectate on death
  UPROPERTY(EditDefaultsOnly, Category = "Rules")
  bool bRespawnImmediately;

  // Set to true to skip round/match timers under editor mode
  UPROPERTY(EditDefaultsOnly, Category = "Debuging")
  bool bSkipMatchTimers;

  // True when warmup is over
  bool bWarmUpTimerOver;

  /** check who won */
  virtual void DetermineMatchWinner();

  /** check if PlayerState is a winner */
  virtual bool IsWinner(class ABBotsPlayerState* PlayerState) const;

  /* Don't allow spectating of enemy teams */
  virtual bool CanSpectate_Implementation(APlayerController* Viewer, APlayerState* ViewTarget) override;

private:
  // The time when the game started
  float gameStartTime;
};



