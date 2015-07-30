// Copyright 2015 VMR Games, Inc. All Rights Reserved.

#include "BattleBots.h"
#include "World/BBotsPlayerStart.h"
#include "Online/BBotsGameInstance.h"
#include "Online/BBotsGameState.h"
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
  GameStateClass = ABBotsGameState::StaticClass();

  MinRespawnDelay = 5.0f;

  //bUseSeamlessTravel = true;
  bWarmUpTimerOver = false;

  // Default is 1 round
  maxNumOfRounds = 1;
  roundTime = 10;
  timeBetweenMatches = 1;
  killScore = 0;
  deathScore = 0;
  bAllowFriendlyFireDamage = false;
}

void ABattleBotsGameMode::PreInitializeComponents()
{
  Super::PreInitializeComponents();

  /* Set timer to run every second */
  GetWorldTimerManager().SetTimer(defaultTimerHandler, this, &ABattleBotsGameMode::DefaultTimer, 1.f/*GetWorldSettings()->GetEffectiveTimeDilation()*/, true);
  GetWorldTimerManager().SetTimer(warmupTimerHandler, this, &ABattleBotsGameMode::WarmUpTimeEnd, warmupTime, false);
}

void ABattleBotsGameMode::DefaultTimer()
{
  // don't update timers for Play In Editor mode, it's not real match
  if (GetWorld()->IsPlayInEditor())
  {
    // start match if necessary.
    if (GetMatchState() == MatchState::WaitingToStart)
    {
      StartMatch();
    }
    return;
  }
  UBBotsGameInstance* const GameInstance = Cast<UBBotsGameInstance>(GetGameInstance());
  ABBotsGameState* const MyGameState = Cast<ABBotsGameState>(GameState);

  if (MyGameState && GameInstance
    && MyGameState->GetRoundsThisMatch() < maxNumOfRounds
    /*&& MyGameState->remainingTime > 0*/
    && !MyGameState->bTimerPaused)
  {
    GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, TEXT("ROUND: ") + FString::FromInt(MyGameState->GetRoundsThisMatch()));
    MyGameState->remainingTime--;

    if (MyGameState->remainingTime <= 0)
    {
       if (MyGameState->GetRoundsThisMatch() < maxNumOfRounds)
       {
        EndOfRoundReset();
        MyGameState->remainingTime = roundTime;
       }
      
      if (GetMatchState() == MatchState::WaitingPostMatch)
      {
        MyGameState->IncRoundsThisMatch();
      }
      else if (GetMatchState() == MatchState::InProgress)
      {
        FinishMatch();

        // Send end round events
        for (FConstControllerIterator It = GetWorld()->GetControllerIterator(); It; ++It)
        {
          ABattleBotsPlayerController* PlayerController = Cast<ABattleBotsPlayerController>(*It);

          if (PlayerController && MyGameState)
          {
            ABBotsPlayerState* PlayerState = Cast<ABBotsPlayerState>((*It)->PlayerState);
            const bool bIsWinner = IsWinner(PlayerState);

            //PlayerController->ClientSendRoundEndEvent(bIsWinner, MyGameState->ElapsedTime);
          }
        }
      }
      else if (GetMatchState() == MatchState::WaitingToStart)
      {
        MyGameState->IncRoundsThisMatch();
        StartMatch();
      }
    }
  }
}

void ABattleBotsGameMode::HandleMatchIsWaitingToStart()
{
  if (bDelayedStart)
  {
    // start warmup if needed
    ABBotsGameState* const MyGameState = Cast<ABBotsGameState>(GameState);
    if (MyGameState && MyGameState->remainingTime == 0)
    {
      const bool bWantsMatchWarmup = !GetWorld()->IsPlayInEditor();
      if (bWantsMatchWarmup && warmupTime > 0)
      {
        MyGameState->remainingTime = warmupTime;
      }
      else
      {
        MyGameState->remainingTime = 0.0f;
      }
    }
  }
}

void ABattleBotsGameMode::HandleMatchHasStarted()
{
  Super::HandleMatchHasStarted();

  ABBotsGameState* const MyGameState = Cast<ABBotsGameState>(GameState);
  if (MyGameState)
  {
    MyGameState->remainingTime = roundTime;
  }

  // Notify players that the game has started
  for (FConstControllerIterator It = GetWorld()->GetControllerIterator(); It; ++It)
  {
    ABattleBotsPlayerController* PC = Cast<ABattleBotsPlayerController>(*It);
    if (PC)
    {
      // Call Blueprintevent to init game started huds
      //PC->ClientGameStarted();
    }
  }
}

void ABattleBotsGameMode::FinishMatch()
{
  ABBotsGameState* const MyGameState = Cast<ABBotsGameState>(GameState);
  if (IsMatchInProgress())
  {
    EndMatch();
    DetermineMatchWinner();

    // Notify players that the game has ended
    for (FConstControllerIterator It = GetWorld()->GetControllerIterator(); It; ++It)
    {
      ABBotsPlayerState* PlayerState = Cast<ABBotsPlayerState>((*It)->PlayerState);
      const bool bIsWinner = IsWinner(PlayerState);

      // Call Blueprintevent to init game ended huds
      //(*It)->GameHasEnded(NULL, bIsWinner);
    }

    /* Lock all pawns. Pawns are not marked as keep for seamless travel,
    /* so we will create new pawns on the next match rather than
    /* turning these back on. */
    for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; ++It)
    {
      (*It)->TurnOff();
    }

    // set up to restart the match
    MyGameState->remainingTime = timeBetweenMatches;
  }
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

bool ABattleBotsGameMode::CanDealDamage(AController* damageInstigator, AController* damagedPlayer) const
{
  if (bAllowFriendlyFireDamage)
    return true;
  
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


bool ABattleBotsGameMode::CanSpectate_Implementation(APlayerController* Viewer, APlayerState* ViewTarget)
{
  ABBotsPlayerState* const ViewerPS = Cast<ABBotsPlayerState>(Viewer->PlayerState);
  ABBotsPlayerState* const ViewTargetPS = Cast<ABBotsPlayerState>(ViewTarget);
  return (ViewerPS && ViewTargetPS && (ViewerPS->GetTeamNum() == ViewTargetPS->GetTeamNum()));
}

void ABattleBotsGameMode::EndOfRoundReset()
{
  for (FActorIterator It(GetWorld()); It; ++It)
  {
    if (It->GetClass()->ImplementsInterface(UBBotsResetInterface::StaticClass()))
    {
      IBBotsResetInterface::Execute_Reset(*It);
    }
  }

  // Destroys all dead bodies; using RESET interface did not delete all of them in time
  for (TObjectIterator<APawn> Itr; Itr; ++Itr)
  {
    if (!Itr->Controller)
    {
      Itr->Destroy();
    }
  }
}

void ABattleBotsGameMode::WarmUpTimeEnd()
{
  EndOfRoundReset();
}

bool ABattleBotsGameMode::ReadyToStartMatch()
{
  if (GetMatchState() == MatchState::InProgress)
  {
    return bWarmUpTimerOver;
  }
  return Super::ReadyToStartMatch();
}

bool ABattleBotsGameMode::ReadyToEndMatch()
{
  //@todo: end match when game timer is up
  return false;
}

AActor* ABattleBotsGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
  // Choose a player start
  ABBotsPlayerStart* FoundPlayerStart = NULL;
  ABBotsPlayerState* PState = Cast<ABBotsPlayerState>(Player->PlayerState);

  for (TActorIterator<ABBotsPlayerStart> It(GetWorld()); It; ++It)
  {
    ABBotsPlayerStart* PlayerStart = *It;

    if (PlayerStart->GetTeamNum() == PState->GetTeamNum())
    {
      // Only spawn at team spawn location
      FoundPlayerStart = PlayerStart;
      return FoundPlayerStart;
    }
  }

  GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("PlayerStart Failed!"));
  // If we can't find a team spot then spawn at a rand location
  return Super::ChoosePlayerStart_Implementation(Player);
}



AActor* ABattleBotsGameMode::FindPlayerStart_Implementation(AController* Player, const FString& IncomingName)
{
  ABBotsPlayerState* PState = Cast<ABBotsPlayerState>(Player->PlayerState);
  for (TActorIterator<ABBotsPlayerStart> It(GetWorld()); It; ++It)
  {
    ABBotsPlayerStart* PlayerStart = *It;

    if (PlayerStart->GetTeamNum() == PState->GetTeamNum())
    {
      // Only spawn at team spawn location
      return PlayerStart;
    }
  }
  // Else return the original spawn spot
  return Super::FindPlayerStart_Implementation(Player, IncomingName);
}




