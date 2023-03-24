// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterState.h"
#include "PlayerCharacterAnimInstance.generated.h"


// Forward declares
class APlayerCharacter;
class UCharacterMovementComponent;

/**
 * Animations for our Player Character
 */
UCLASS()
class LIMITLESS_API UPlayerCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	// Variables to expose to Blueprint Event Graph using BlueprintReadOnly
 	UPROPERTY(BlueprintReadOnly)
	APlayerCharacter* PlayerCharacter;

	UPROPERTY(BlueprintReadOnly)
	UCharacterMovementComponent* PlayerCharacterMovementComponent;

	UPROPERTY(BlueprintReadOnly)
	float MovementSpeed; 

	UPROPERTY(BlueprintReadOnly)
	bool bIsFalling;

	UPROPERTY(BlueprintReadOnly)
	ECharacterState CharacterState;


	// Override animation functions
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;
	
};
