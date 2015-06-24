// Copyright 2015 VMR Games, Inc. All Rights Reserved.

#include "BattleBots.h"
#include "BattleBotsGameMode.h"
#include "Online/BBotsSpectatorPawn.h"
#include "BattleBotsPlayerController.h"
#include "BattleBotsCharacter.h"

ABattleBotsGameMode::ABattleBotsGameMode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
  // use our custom PlayerController class
  PlayerControllerClass = ABattleBotsPlayerController::StaticClass();

  // set default pawn class to our Blueprinted character
  static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/BBotCharacter/Sorcerer"));
  if (PlayerPawnBPClass.Class != NULL)
  {
    DefaultPawnClass = PlayerPawnBPClass.Class;
  }

  PlayerControllerClass = ABattleBotsPlayerController::StaticClass();
  PlayerStateClass = ABBotsPlayerState::StaticClass();
  SpectatorClass = ABBotsSpectatorPawn::StaticClass();
  //GameStateClass = ABBotsGameState::StaticClass();

  MinRespawnDelay = 5.0f;

  bUseSeamlessTravel = true;

  roundTime = 10;
  timeBetweenMatches = 1;
  killScore = 0;
  deathScore = 0;
  damageSelfScale = 1;
  bAllowFriendlyFireDamage = false;
}


void ABattleBotsGameMode::DetermineMatchWinner()
{
  // nothing to do here
}

bool ABattleBotsGameMode::IsWinner(class ABBotsPlayerState* PlayerState) const
{
  return false;
}

void ABattleBotsGameMode::Killed(AController* killer, AController* killedPlayer, APawn* killedPawn, const TSubclassOf<UDamageType> DamageType)
{
  ABBotsPlayerState* killerPlayerState = killer ? Cast<ABBotsPlayerState>(killer->PlayerState) : NULL;
  ABBotsPlayerState* victimPlayerState = killedPlayer ? Cast<ABBotsPlayerState>(killedPlayer->PlayerState) : NULL;

  if (killerPlayerState && killerPlayerState != victimPlayerState)
  {
    killerPlayerState->ScoreKill(victimPlayerState, killScore);
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("Kills: ") + FString::FromInt(killerPlayerState->GetKills()));
  }
  
  if (victimPlayerState)
  {
    victimPlayerState->ScoreDeath(killerPlayerState, deathScore);
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("Character Died: ") + FString::FromInt(victimPlayerState->GetDeaths()));
  }
}

bool ABattleBotsGameMode::CanDealDamage(class ABBotsPlayerState* damageInstigator, class ABBotsPlayerState* damagedPlayer) const
{
  if (bAllowFriendlyFireDamage)
  {
    return true;
  }
  return damageInstigator && damagedPlayer && (damageInstigator->GetTeamNum() != damagedPlayer->GetTeamNum());
}

bool ABattleBotsGameMode::ShouldSpawnAtStartSpot(AController* Player)
{
  return false;
}

bool ABattleBotsGameMode::CanDealDamageTest(AController* damageInstigator, AController* damagedPlayer) const
{
  ABBotsPlayerState* killerPlayerState = damageInstigator ? Cast<ABBotsPlayerState>(damageInstigator->PlayerState) : NULL;
  ABBotsPlayerState* victimPlayerState = damagedPlayer ? Cast<ABBotsPlayerState>(damagedPlayer->PlayerState) : NULL;

  if (killerPlayerState && victimPlayerState && (killerPlayerState->GetTeamNum() == victimPlayerState->GetTeamNum()))
  {
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("We are the same team!!!: "));
  }
  return killerPlayerState && victimPlayerState && (killerPlayerState->GetTeamNum() != victimPlayerState->GetTeamNum());
}

bool ABattleBotsGameMode::CanRespawnImmediately()
{
  return bRespawnImmediately;
}

void ABattleBotsGameMode::HandleMatchIsWaitingToStart()
{
//   if (bDelayedStart)
//   {
//     // start warmup if needed
//     AShooterGameState* const MyGameState = Cast<AShooterGameState>(GameState);
//     if (MyGameState && MyGameState->RemainingTime == 0)
//     {
//       const bool bWantsMatchWarmup = !GetWorld()->IsPlayInEditor();
//       if (bWantsMatchWarmup && WarmupTime > 0)
//       {
//         MyGameState->RemainingTime = WarmupTime;
//       }
//       else
//       {
//         MyGameState->RemainingTime = 0.0f;
//       }
//     }
//   }
}

void ABattleBotsGameMode::HandleMatchHasStarted()
{
    Super::HandleMatchHasStarted();

//     ABBotsGameState* const MyGameState = Cast<ABBotsGameState>(GameState);
//     MyGameState->RemainingTime = RoundTime;
// 
//     // notify players
//     for (FConstControllerIterator It = GetWorld()->GetControllerIterator(); It; ++It)
//     {
//       ABattleBotsPlayerController* PC = Cast<ABattleBotsPlayerController>(*It);
//       if (PC)
//       {
//         PC->ClientGameStarted();
//       }
//     }
}

bool ABattleBotsGameMode::CanSpectate(APlayerController* Viewer, APlayerState* ViewTarget)
{
  ABBotsPlayerState* const ViewerPS = Cast<ABBotsPlayerState>(Viewer->PlayerState);
  ABBotsPlayerState* const ViewTargetPS = Cast<ABBotsPlayerState>(ViewTarget);
  return (ViewerPS && ViewTargetPS && (ViewerPS->GetTeamNum() != ViewTargetPS->GetTeamNum()));
}