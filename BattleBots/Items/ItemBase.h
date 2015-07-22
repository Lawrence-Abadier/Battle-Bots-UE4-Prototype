// Copyright 2015 VMR Games, Inc. All Rights Reserved.

#pragma once

#include "GameFramework/Actor.h"
#include "ItemBase.generated.h"

UENUM(BlueprintType)
enum class EItemSlot :uint8{
  EHelm       UMETA(DisplayName = "Helm"),
  EChest      UMETA(DisplayName = "Chest"),
  ELegs       UMETA(DisplayName = "Legs"),
  EFeet       UMETA(DisplayName = "Feet"),
  EMainHand   UMETA(DisplayName = "MainHand"),
  EOffHand    UMETA(DisplayName = "OffHand"),
};

UCLASS()
class BATTLEBOTS_API AItemBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemBase();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

  // Equips the item 
  void EquipItem();

  // UnEquip the item
  void UnEquipItem();
	
};
