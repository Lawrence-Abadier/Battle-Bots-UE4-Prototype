// Copyright 2015 VMR Games, Inc. All Rights Reserved.

#pragma once

#include "GameFramework/PlayerState.h"
#include "BBotsPlayerState.generated.h"

/**
 *
 */
UCLASS()
class BATTLEBOTS_API ABBotsPlayerState : public APlayerState
{
  GENERATED_BODY()

public:
  ABBotsPlayerState(const FObjectInitializer& ObjectInitializer);

  // Begin APlayerState interface
  /** clear scores */
  virtual void Reset() override;

  /**
  * Set the team
  *
  * @param	InController	The controller to initialize state with
  */
  virtual void ClientInitialize(class AController* InController) override;

  virtual void UnregisterPlayerWithSession() override;

  // End APlayerState interface

  /**
  * Set new team and update pawn. Also updates player character team colors.
  *
  * @param	NewTeamNumber	Team we want to be on.
  */
  void SetTeamNum(int32 NewTeamNumber);

  /** player killed someone */
  void ScoreKill(ABBotsPlayerState* Victim, int32 Points);

  /** player died */
  void ScoreDeath(ABBotsPlayerState* KilledBy, int32 Points);

  void ScorePoints(int32 Points);

  /** get current team */
  UFUNCTION(BlueprintCallable, Category = "PlayerState")
  int32 GetTeamNum() const;

  /** get number of kills */
  UFUNCTION(BlueprintCallable, Category = "PlayerState")
  int32 GetKills() const;

  /** get number of deaths */
  UFUNCTION(BlueprintCallable, Category = "PlayerState")
  int32 GetDeaths() const;

  /** replicate team colors. Updated the players mesh colors appropriately */
  UFUNCTION()
  void OnRep_TeamColor();

protected:

  /** Set the mesh colors based on the current teamnum variable */
  void UpdateTeamColors();

  /** team number */
  UPROPERTY(Transient, ReplicatedUsing = OnRep_TeamColor)
  int32 teamNumber;

  /** number of kills */
  UPROPERTY(Transient, Replicated)
  int32 numKills;

  /** number of deaths */
  UPROPERTY(Transient, Replicated)
  int32 numDeaths;

};
