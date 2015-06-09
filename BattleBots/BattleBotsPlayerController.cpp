// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "BattleBots.h"
#include "Character/BBotCharacter.h"
#include "BattleBotsPlayerController.h"
#include "AI/Navigation/NavigationSystem.h"

ABattleBotsPlayerController::ABattleBotsPlayerController(const FObjectInitializer& ObjectInitializer)
  : Super(ObjectInitializer)
{
  bRotChanged = false;
  bShowMouseCursor = true;
  DefaultMouseCursor = EMouseCursor::Crosshairs;

  // Allows the character to rotate to the mouse direction
  rotObjTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic));
  rotObjTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));

  // Gets a location on the ground to spawn the aoe spell
  aoeObjTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic));
}

void ABattleBotsPlayerController::BeginPlay()
{
  Super::BeginPlay();

}

void ABattleBotsPlayerController::PlayerTick(float DeltaTime)
{
  Super::PlayerTick(DeltaTime);

  // keep updating the destination every tick while desired
  MoveToMouseCursor();

  if (playerCharacter && bRotChanged)
  {
    playerCharacter->SetActorRotation(localRotation);
  }
}

void ABattleBotsPlayerController::SetupInputComponent()
{
  // set up gameplay key bindings
  Super::SetupInputComponent();

  InputComponent->BindAction("SetDestination", IE_Pressed, this, &ABattleBotsPlayerController::OnSetDestinationPressed);
  InputComponent->BindAction("SetDestination", IE_Released, this, &ABattleBotsPlayerController::OnSetDestinationReleased);

  InputComponent->BindAction("CastSpellOnRightClick", IE_Pressed, this, &ABattleBotsPlayerController::CastOnRightClick);
  InputComponent->BindAction("CastSpellOnRightClick", IE_Released, this, &ABattleBotsPlayerController::OnRotatationEnd);

  InputComponent->BindAction("HotBarSlot_1", IE_Pressed, this, &ABattleBotsPlayerController::HotBarSlot_One);
  InputComponent->BindAction("HotBarSlot_1", IE_Released, this, &ABattleBotsPlayerController::OnRotatationEnd);

  InputComponent->BindAction("HotBarSlot_2", IE_Pressed, this, &ABattleBotsPlayerController::HotBarSlot_Two);
  InputComponent->BindAction("HotBarSlot_2", IE_Released, this, &ABattleBotsPlayerController::OnRotatationEnd);

  InputComponent->BindAction("HotBarSlot_3", IE_Pressed, this, &ABattleBotsPlayerController::HotBarSlot_Three);
  InputComponent->BindAction("HotBarSlot_3", IE_Released, this, &ABattleBotsPlayerController::OnRotatationEnd);

  InputComponent->BindAction("HotBarSlot_4", IE_Pressed, this, &ABattleBotsPlayerController::HotBarSlot_Four);
  InputComponent->BindAction("HotBarSlot_4", IE_Released, this, &ABattleBotsPlayerController::OnRotatationEnd);
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
  ABBotCharacter* CurrentPawn = ReferencePossessedPawn();

  return CurrentPawn == Character;
}

ABBotCharacter* ABattleBotsPlayerController::ReferencePossessedPawn()
{
  return Cast<ABBotCharacter>(this->GetPawn());
}

void ABattleBotsPlayerController::CastFromSpellBarIndex(int32 index)
{
  RotateToMouseCursor();
  if (playerCharacter)
  {
    playerCharacter->CastFromSpellBar(index, GetMouseHitLocation(aoeObjTypes));
  }
}

void ABattleBotsPlayerController::CastOnRightClick()
{
  CastFromSpellBarIndex(0);
}

void ABattleBotsPlayerController::HotBarSlot_One()
{
  CastFromSpellBarIndex(0);
}

void ABattleBotsPlayerController::HotBarSlot_Two()
{
  CastFromSpellBarIndex(1);
}

void ABattleBotsPlayerController::HotBarSlot_Three()
{
  CastFromSpellBarIndex(2);
}

void ABattleBotsPlayerController::HotBarSlot_Four()
{
  CastFromSpellBarIndex(3);
}


FVector ABattleBotsPlayerController::GetMouseHitLocation(const TArray<TEnumAsByte<EObjectTypeQuery> >& ObjTypes)
{
  // Get hit location under mouse click
  FHitResult hit;
  // Get hit result under object types (Ex: Walls, Floor, Pawn, etc)
  GetHitResultUnderCursorForObjects(ObjTypes, false, hit);

  return hit.Location;
}

void ABattleBotsPlayerController::RotateToMouseCursor()
{
  // Replicate local actor rotation as soon as the button is clicked
  bRotChanged = true;

  playerCharacter = ReferencePossessedPawn();

  if (playerCharacter) {
    // Get hit location under mouse click
    FVector mouseHitLoc = GetMouseHitLocation(rotObjTypes);
    FVector characterLoc = playerCharacter->GetActorLocation();

    // Get the target location direction
    FVector targetLoc = (mouseHitLoc - characterLoc);
    targetLoc.Normalize();

    DrawDebugLine(GetWorld(), characterLoc, mouseHitLoc, FColor::Green, true, 0.5f, 0, 2.f);
    FRotator newRotation(0.f, targetLoc.Rotation().Yaw, 0.f);

    // Stop current movement,and face the new direction
    StopMovement();
    // Set the rotation on the server
    ServerRotateToMouseCursor(newRotation);
  }
}

void ABattleBotsPlayerController::ServerRotateToMouseCursor_Implementation(FRotator newRotation)
{
  playerCharacter = ReferencePossessedPawn();
  // Set the actor rotation on the server
  playerCharacter->SetActorRotation(newRotation);
  // Update the local actor rotation
  localRotation = newRotation;
}

bool ABattleBotsPlayerController::ServerRotateToMouseCursor_Validate(FRotator newRotation)
{
  return true;
}

// Stop player rotation on button release
void ABattleBotsPlayerController::OnRotatationEnd()
{
  bRotChanged = false;
}

void ABattleBotsPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
  Super::GetLifetimeReplicatedProps(OutLifetimeProps);

  // Value is already updated locally, so we may skip it in replication step for the owner only
  DOREPLIFETIME_CONDITION(ABattleBotsPlayerController, localRotation, COND_OwnerOnly);
}




