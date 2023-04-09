// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

// Equipment types

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	ECS_Unarmed UMETA(DisplayName = "Unarmed"),
	ECS_OneHanded UMETA(DisplayName = "OneHanded"),
	ECS_TwoHanded UMETA(DisplayName = "TwoHanded")
};


// States for the player character

UENUM(BlueprintType)
enum class EActionState : uint8
{
	EAS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	EAS_Attacking UMETA(DisplayName = "Attacking"),
	// General Occupied
	EAS_Occupied UMETA(DisplayName = "Occupied")
};

// States for the enemy AI to potentially be in.

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	EES_Patrolling UMETA(DisplayName = "Patrolling"),
	EES_Chasing UMETA(DisplayName = "Chasing"),
	EES_Attacking UMETA(DisplayName = "Attacking")
};