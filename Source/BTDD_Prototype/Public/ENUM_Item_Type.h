// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ENUM_Item_Type.generated.h"

UENUM(BlueprintType)
enum ItemType 
{
	Door = 0 UMETA(DisplayName = "Door"),
	Pickups = 1 UMETA(DisplayName = "Pickups"),
	Drawer = 2 UMETA(DisplayName = "Drawer"),
	Interaction_Character = 3 UMETA(DisplayName = "Character Interaction")
};