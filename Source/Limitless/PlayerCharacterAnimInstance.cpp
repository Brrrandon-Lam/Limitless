// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacterAnimInstance.h"
#include "PlayerCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"


// Here we want to initialize things for our animation
void UPlayerCharacterAnimInstance::NativeInitializeAnimation() {

	Super::NativeInitializeAnimation();
	// Get and assign a pointer variable to our character and movement component
	PlayerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());
	// If we successfully get the pawn owner, get the movement component too.
	if (PlayerCharacter) 
	{
		PlayerCharacterMovementComponent = PlayerCharacter->GetCharacterMovement();
	}
}

// Update the animation every tick essentially.
void UPlayerCharacterAnimInstance::NativeUpdateAnimation(float DeltaTime) {
	Super::NativeUpdateAnimation(DeltaTime);

	// If we successfully get the movement component, set the ground speed.
	if (PlayerCharacterMovementComponent) 
	{
		// Get the character's movement speed
		MovementSpeed = UKismetMathLibrary::VSizeXY(PlayerCharacterMovementComponent->Velocity);
		// Check are we falling?
		bIsFalling = PlayerCharacterMovementComponent->IsFalling();
		// Get the character's state and store it for use in our animation.
		CharacterState = PlayerCharacter->GetCharacterState();
	}
}
