// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "BattleBots.h"
#include "Character/BBotCharacter.h"
#include "BattleBotsPlayerController.h"
#include "AI/Navigation/NavigationSystem.h"

ABattleBotsPlayerController::ABattleBotsPlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void ABattleBotsPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	// keep updating the destination every tick while desired
	MoveToMouseCursor();
}

void ABattleBotsPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("SetDestination", IE_Pressed, this, &ABattleBotsPlayerController::OnSetDestinationPressed);
	InputComponent->BindAction("SetDestination", IE_Released, this, &ABattleBotsPlayerController::OnSetDestinationReleased);
	InputComponent->BindAction("CastSpellOnRightClick", IE_Pressed, this, &ABattleBotsPlayerController::CastOnRightClick);

	// support touch devices 
	InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &ABattleBotsPlayerController::MoveToTouchLocation);
	InputComponent->BindTouch(EInputEvent::IE_Repeat, this, &ABattleBotsPlayerController::MoveToTouchLocation);
}

void ABattleBotsPlayerController::MoveToMouseCursor()
{
	// Trace to see what is under the mouse cursor
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, false, Hit);

  if (bMoveToMouseCursor && Hit.bBlockingHit)
	{
		// We hit something, move there
		SetNewMoveDestination(Hit.ImpactPoint);
	}
}

void ABattleBotsPlayerController::MoveToTouchLocation(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	FVector2D ScreenSpaceLocation(Location);

	// Trace to see what is under the touch location
	FHitResult HitResult;
	//GetHitResultAtScreenPosition(ScreenSpaceLocation, CurrentClickTraceChannel, true, HitResult);
  GetHitResultUnderCursor(ECC_Visibility, false, HitResult);
  if (HitResult.bBlockingHit)
	{
		// We hit something, move there
		SetNewMoveDestination(HitResult.ImpactPoint);
	}
}

void ABattleBotsPlayerController::SetNewMoveDestination(const FVector DestLocation)
{
	APawn* const Pawn = GetPawn();
	if (Pawn)
	{
		//UNavigationSystem* const NavSys = GetWorld()->GetNavigationSystem();
		float const Distance = FVector::Dist(DestLocation, Pawn->GetActorLocation());
    FVector const Direction = (DestLocation - Pawn->GetActorLocation()).Rotation().Vector();

    Pawn->AddMovementInput(Direction, Distance);
// 		// We need to issue move command only if far enough in order for walk animation to play correctly
// 		if (NavSys && (Distance > 120.0f))
// 		{
// 			NavSys->SimpleMoveToLocation(this, DestLocation);
// 		}
	}
}

void ABattleBotsPlayerController::OnSetDestinationPressed()
{
	// set flag to keep updating destination until released
	bMoveToMouseCursor = true;
}

void ABattleBotsPlayerController::OnSetDestinationReleased()
{
	// clear flag to indicate we should stop updating the destination
	bMoveToMouseCursor = false;
}

bool ABattleBotsPlayerController::IsPossessedBy(ABBotCharacter* Character)
{
	ABBotCharacter* CurrentPawn = Cast<ABBotCharacter>(this->GetPawn());

	return CurrentPawn == Character;
}

void ABattleBotsPlayerController::CastOnRightClick()
{
	RotateToMouseCursor();
	ABBotCharacter* const playerCharacter = Cast<ABBotCharacter>(this->GetPawn());
	if (playerCharacter)
	{
		playerCharacter->CastFromSpellBar(0);
	}
}

void ABattleBotsPlayerController::RotateToMouseCursor()
{
	ABBotCharacter* const playerCharacter = Cast<ABBotCharacter>(this->GetPawn());
	if (playerCharacter) {
		// Get hit location under mouse click
		FHitResult hit;
		GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Visibility), true, hit);

		FVector mouseHitLoc = hit.Location;
		FVector characterLoc = playerCharacter->GetActorLocation();

		// Get the target location direction
		FVector targetLoc = (mouseHitLoc - characterLoc);
		targetLoc.Normalize();

		DrawDebugLine(GetWorld(), characterLoc, mouseHitLoc, FColor::Green, true, 0.5f, 0, 2.f);
		FRotator newRotation(0.f, targetLoc.Rotation().Yaw, 0.f);

		// Stop current movement,and face the new direction
		StopMovement();
		playerCharacter->SetActorRotation(newRotation);
	}
}
