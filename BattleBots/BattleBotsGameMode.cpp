// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "BattleBots.h"
#include "BattleBotsGameMode.h"
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
}