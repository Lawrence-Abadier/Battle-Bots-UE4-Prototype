// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "BattleBots.h"
#include "UI/ChatBlockWidget.h"
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

  // Init default values on the server
  InitPostRoundReset();
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

ABBotCharacter* ABattleBotsPlayerController::ReferencePossessedPawn()
{
  return Cast<ABBotCharacter>(this->GetPawn());
}

void ABattleBotsPlayerController::CastFromSpellBarIndex(int32 index)
{
  /* If the server calls this function, the spell will spawn at the 
  original pawns dead body. Thus, we need to update the char reference.*/
  if (!playerCharacter || HasAuthority())
    ServerReferencePawn();

  // We short-circuit if we can cast to prevent unnecessary calls
  if (playerCharacter && playerCharacter->CanCast(index))
  {
    RotateToMouseCursor();
    playerCharacter->CastFromSpellBar(index, GetLineOfSightImpactPoint());
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
  GetHitResultUnderCursorForObjects(ObjTypes, true, hit);

  DrawDebugLine(GetWorld(), playerCharacter->GetActorLocation(), hit.ImpactPoint, FColor::Red, true, -1.0f, 0, 2.f);

  //  We need to extend the line trace to avoid returning zero::vectors from not reaching the target
  return hit.ImpactPoint + (hit.ImpactNormal * -1.0f);
}

FHitResult ABattleBotsPlayerController::SingleLineTrace(const FVector& Start, const FVector& End)
{
  FCollisionQueryParams traceParams = FCollisionQueryParams(FName(TEXT("LOS_Trace")), true, this);
  traceParams.bTraceComplex = true;
  traceParams.bTraceAsyncScene = true;
  traceParams.bReturnPhysicalMaterial = false;

  //Re-initialize hit info
  FHitResult hitResult(ForceInit);

  //call GetWorld() from within an actor extending class
  GetWorld()->LineTraceSingleByChannel(
    hitResult,        //result
    Start,    //start
    End, //end
    ECC_Visibility, //collision channel
    traceParams
    );

  DrawDebugLine(GetWorld(), Start, hitResult.ImpactPoint, FColor::Green, true, -1.0f, 0, 2.f);
  return hitResult;
}

FVector ABattleBotsPlayerController::GetLineOfSightImpactPoint()
{
  if (!playerCharacter)
  {
    return FVector::ZeroVector;
  }
  
  FHitResult hitResult = SingleLineTrace(playerCharacter->GetActorLocation(), GetMouseHitLocation(aoeObjTypes));
  
  // Return the hit location if the impact point is null
  return hitResult.ImpactPoint.Equals(FVector::ZeroVector) ? hitResult.Location : hitResult.ImpactPoint;
}

void ABattleBotsPlayerController::RotateToMouseCursor()
{
  // Replicate local actor rotation as soon as the button is clicked
  bRotChanged = true;
  
  if (playerCharacter) {
    // Get hit location under mouse click
    FVector mouseHitLoc = GetMouseHitLocation(rotObjTypes);
    FVector characterLoc = playerCharacter->GetActorLocation();

    // Get the target location direction
    FVector targetLoc = (mouseHitLoc - characterLoc);
    targetLoc.Normalize();

    //DrawDebugLine(GetWorld(), characterLoc, mouseHitLoc, FColor::Green, true, 0.5f, 0, 2.f);
    FRotator newRotation(0.f, targetLoc.Rotation().Yaw, 0.f);

    // Stop current movement,and face the new direction
    StopMovement();
    // Set the rotation on the server
    ServerRotateToMouseCursor(newRotation);
  }
}

void ABattleBotsPlayerController::ServerRotateToMouseCursor_Implementation(FRotator newRotation)
{
  if (!playerCharacter)
    return;

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

void ABattleBotsPlayerController::PawnPendingDestroy(APawn* deadPawn)
{
  // Get death location to setup death cam
  CameraLocation = Cast<ABBotCharacter>(deadPawn)->GetTopDownCameraComponent()->GetComponentToWorld().GetLocation();
  CameraRotation= FRotator(-60.0f, 0.0f, 0.0f);
  
  Super::PawnPendingDestroy(deadPawn);

  //@todo: check gamestate if round has ended
  if (bCanRespawnInstantly)
  {
    // Add a delay to prevent D/C from attempting to respawn too quickly
    GetWorldTimerManager().SetTimer(RespawnHandler, this, &ABattleBotsPlayerController::RespawnPlayer, 0.1, false);
  }
  else
  {
    // Set the respawn timer and start spectating
    GetWorldTimerManager().SetTimer(RespawnHandler, this, &ABattleBotsPlayerController::RespawnPlayer, RespawnTime, false);
    //Scale the respawn timer per death
    RespawnTime *= RespawnDeathScale;
    StartSpectating();
  }
}

float ABattleBotsPlayerController::GetTimeTillSpawn()
{
  return GetWorldTimerManager().GetTimerRemaining(RespawnHandler);
}

void ABattleBotsPlayerController::RespawnPlayer()
{
  currGM->RestartPlayer(this);
  GetWorldTimerManager().ClearTimer(RespawnHandler);
}

void ABattleBotsPlayerController::StartSpectating()
{
  // Update state on the server
  PlayerState->bIsSpectator = true;
  // Waiting to respawn
  bPlayerIsWaiting = true;
  ChangeState(NAME_Spectating);
  // Update state for the client
  ClientGotoState(NAME_Spectating);

  // Focus camera on the remaining player
  ViewAPlayer(1);
  //@todo: if no teammate alive, create a static spectate spot
  //@todo: update hud to set spectating
}


void ABattleBotsPlayerController::ViewAPlayer(int32 dir)
{
  // Find a teammate to spectate
  APlayerState* const PlayerState = GetNextViewablePlayer(dir);

  if (PlayerState != NULL)
  {
    // Found a teammate to spectate
    SetViewTarget(PlayerState);
  }
  else
  {
    // Death Cam
    ClientSetSpectatorCamera(CameraLocation, CameraRotation);
  }
}

void ABattleBotsPlayerController::ClientSetSpectatorCamera_Implementation(FVector CameraLocation, FRotator CameraRotation)
{
  SetInitialLocationAndRotation(CameraLocation, CameraRotation);
  SetViewTarget(this);
}

void ABattleBotsPlayerController::Reset()
{
  if (HasAuthority())
  {
	  bCanRespawnInstantly = true;
	  if (GetPawn())
	  {
	    GetPawn()->Reset();
	  }
	  else
	  {
	    RespawnPlayer();
	  }
    InitPostRoundReset();
  }
}

void ABattleBotsPlayerController::InitPostRoundReset()
{
  if (HasAuthority())
  {
    currGM = GetWorld()->GetAuthGameMode<ABattleBotsGameMode>();
    if (currGM)
    {
      bCanRespawnInstantly = currGM->CanRespawnImmediately();
      RespawnTime = currGM->GetRespawnTime();
      RespawnDeathScale = currGM->GetRespawnDeathScale();
    }
    // On_Rep functions dont get called on the server, so they must be called manually
    ServerReferencePawn();
  }
}
void ABattleBotsPlayerController::Reset_Implementation()
{
  if (HasAuthority())
  {
    APawn* const MyPawn = GetPawn();
    if (MyPawn)
    {
      MyPawn->TurnOff();
    }
    
    GetWorldTimerManager().SetTimer(RespawnHandler, this, &ABattleBotsPlayerController::Reset, 1.0, false);
  }
}

void ABattleBotsPlayerController::OnRep_Pawn()
{
  Super::OnRep_Pawn();

  // On_Rep functions dont get called on the server
  ServerReferencePawn();
}

void ABattleBotsPlayerController::ServerReferencePawn_Implementation()
{
  playerCharacter = ReferencePossessedPawn();
}

bool ABattleBotsPlayerController::ServerReferencePawn_Validate()
{
  return true;
}

void ABattleBotsPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
  Super::GetLifetimeReplicatedProps(OutLifetimeProps);

  // Value is already updated locally, so we may skip it in replication step for the owner only
  DOREPLIFETIME_CONDITION(ABattleBotsPlayerController, localRotation, COND_OwnerOnly);
  DOREPLIFETIME_CONDITION(ABattleBotsPlayerController, playerCharacter, COND_OwnerOnly);
}



