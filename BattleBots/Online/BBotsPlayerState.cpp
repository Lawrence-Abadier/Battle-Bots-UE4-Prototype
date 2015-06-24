// Copyright 2015 VMR Games, Inc. All Rights Reserved.

#include "BattleBots.h"
#include "BBotsPlayerState.h"


ABBotsPlayerState::ABBotsPlayerState(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
  teamNumber = 0;
  numKills = 0;
  numDeaths = 0;
}

void ABBotsPlayerState::Reset()
{
  Super::Reset();

  //PlayerStates persist across seamless travel.  Keep the same teams as previous match.
  //SetTeamNum(0);
  numKills = 0;
  numDeaths = 0;
}

void ABBotsPlayerState::UnregisterPlayerWithSession()
{
  if (!bFromPreviousLevel)
  {
    Super::UnregisterPlayerWithSession();
  }
}

void ABBotsPlayerState::ClientInitialize(class AController* InController)
{
  Super::ClientInitialize(InController);

  UpdateTeamColors();
}

void ABBotsPlayerState::SetTeamNum(int32 NewTeamNumber)
{
  teamNumber = NewTeamNumber;

  UpdateTeamColors();
}

void ABBotsPlayerState::OnRep_TeamColor()
{
  UpdateTeamColors();
}

void ABBotsPlayerState::UpdateTeamColors()
{
//   AController* OwnerController = Cast<AController>(GetOwner());
//   if (OwnerController != NULL)
//   {
//     AShooterCharacter* ShooterCharacter = Cast<AShooterCharacter>(OwnerController->GetCharacter());
//     if (ShooterCharacter != NULL)
//     {
//       ShooterCharacter->UpdateTeamColorsAllMIDs();
//     }
//   }
}

int32 ABBotsPlayerState::GetTeamNum() const
{
  return teamNumber;
}

int32 ABBotsPlayerState::GetKills() const
{
  return numKills;
}

int32 ABBotsPlayerState::GetDeaths() const
{
  return numDeaths;
}

void ABBotsPlayerState::ScoreKill(ABBotsPlayerState* Victim, int32 Points)
{
  numKills++;
  //ScorePoints(Points);
}

void ABBotsPlayerState::ScoreDeath(ABBotsPlayerState* KilledBy, int32 Points)
{
  numDeaths++;
  //ScorePoints(Points);
}

//void ABBotsPlayerState::ScorePoints(int32 Points)
//{
//   ABBotsPlayerState* const MyGameState = Cast<ABBotsPlayerState>(GetWorld()->GameState);
//   if (MyGameState && teamNumber >= 0)
//   {
//     if (teamNumber >= MyGameState->TeamScores.Num())
//     {
//       MyGameState->TeamScores.AddZeroed(teamNumber - MyGameState->TeamScores.Num() + 1);
//     }
// 
//     MyGameState->TeamScores[teamNumber] += Points;
//   }
// 
//   Score += Points;
//}

void ABBotsPlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
  Super::GetLifetimeReplicatedProps(OutLifetimeProps);

  DOREPLIFETIME(ABBotsPlayerState, teamNumber);
  DOREPLIFETIME(ABBotsPlayerState, numKills);
  DOREPLIFETIME(ABBotsPlayerState, numDeaths);
}


