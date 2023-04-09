// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GroomComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Items/Item.h"
#include "Animation/AnimMontage.h"
#include "Items/Weapon.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Initialize character as unarmed
	CharacterState = ECharacterState::ECS_Unarmed;
	CharacterActionState = EActionState::EAS_Unoccupied;

	// Don't let the character rotate with the control rotation.
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	// Make the character orient the rotation towards movement, with a rotation rate of 400.
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	

	// Initialize our Camera and Camera Boom
	CameraBoom = CreateDefaultSubobject <USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 250.f;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraBoom);

	// Initialize Hair and Eyebrows
	Hair = CreateDefaultSubobject<UGroomComponent>(TEXT("Hair"));
	Hair->SetupAttachment(GetMesh());
	Hair->AttachmentName = FString("head");

	Eyebrows = CreateDefaultSubobject<UGroomComponent>(TEXT("Eyebrows"));
	Eyebrows->SetupAttachment(GetMesh());
	Eyebrows->AttachmentName = FString("head");

	// Set Tags
	

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	// Adds a player tag to our character.
	Tags.Add(FName("Player"));
	
}

void APlayerCharacter::EquipButtonPressed()
{
	AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem);
	if (OverlappingWeapon) 
	{
		OverlappingWeapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);
		// We only have a 2H weapon, so for now just set the state to 2H
		CharacterState = ECharacterState::ECS_TwoHanded;
		// Set the equipped weapon
		EquippedWeapon = OverlappingWeapon;
	}
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// Initialize WASD controls, Jump.
	// Axes are: MoveForward, MoveRight, LookUp, LookRight
	
	PlayerInputComponent->BindAxis(FName("MoveForward"), this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis(FName("MoveRight"), this, &APlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis(FName("LookUp"), this, &APlayerCharacter::LookUp);
	PlayerInputComponent->BindAxis(FName("LookRight"), this, &APlayerCharacter::LookRight);

	// Use ACharacter's built in Jump function.
	PlayerInputComponent->BindAction(FName("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	// Equip
	PlayerInputComponent->BindAction(FName("Equip"), EInputEvent::IE_Pressed, this, &APlayerCharacter::EquipButtonPressed);
	// Attack bind to left click
	PlayerInputComponent->BindAction(FName("Attack"), EInputEvent::IE_Pressed, this, &APlayerCharacter::Attack);
	// Attack bind to right click
	PlayerInputComponent->BindAction(FName("HeavyAttack"), EInputEvent::IE_Pressed, this, &APlayerCharacter::HeavyAttack);
	// Sheathe
	PlayerInputComponent->BindAction(FName("Sheathe"), EInputEvent::IE_Pressed, this, &APlayerCharacter::Sheathe);
}

// Find the controller's forward vector and apply movement in that direction.
void APlayerCharacter::MoveForward(float Value) 
{
	if (CharacterActionState == EActionState::EAS_Unoccupied && Controller && Value != 0.f) 
	{
		// Get the Yaw (Z-Component) of the control rotation
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);
		// Find the forward axis, which is the X axis of the control rotation's yaw.
		const FVector ControllerForwardVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(ControllerForwardVector, Value);
	}
}

// Find the controller's right vector and apply movement in that direction.
void APlayerCharacter::MoveRight(float Value) 
{
	if (CharacterActionState == EActionState::EAS_Unoccupied && Controller && Value != 0.f) 
	{
		
		// Get the Yaw of the control rotation
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);
		// Find the right axis, which is the Y axis of the control rotation's yaw.
		const FVector ControllerRightVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(ControllerRightVector, Value);
	}
}

// Use Controller rotation to make the character look.

void APlayerCharacter::LookUp(float Value) 
{
	AddControllerPitchInput(Value);
	
}

void APlayerCharacter::LookRight(float Value) 
{
	AddControllerYawInput(Value);
}

void APlayerCharacter::Attack() 
{
	// Get the animation instance
	UAnimInstance* CurrentInstance = GetMesh()->GetAnimInstance();
	// If AttackMontage is set && current instance exists && we have a one or twohanded weapon equipped.
	if (CurrentInstance && CanAttack()) 
	{
		// Play the attack montage, jumping to section attack1.
		CharacterActionState = EActionState::EAS_Attacking;
		CurrentInstance->Montage_Play(AttackMontage);
		CurrentInstance->Montage_JumpToSection(FName("Attack1"), AttackMontage);
	}
}

void APlayerCharacter::HeavyAttack()
{
	UAnimInstance* CurrentInstance = GetMesh()->GetAnimInstance();
	if (CurrentInstance && CanAttack())
	{
		// Play the heavy attack section of the attack montage, set state to attacking.
		CharacterActionState = EActionState::EAS_Attacking;
		CurrentInstance->Montage_Play(AttackMontage);
		CurrentInstance->Montage_JumpToSection(FName("HeavyAttack1"), AttackMontage);
	}
}

/* Validate whether the character is in a state that allows them to attack or not 
*  Current Requirements: Montage must be set, Character must have a weapon type equipped, Action state must be unoccupied.
*
*/
bool APlayerCharacter::CanAttack() 
{
	return AttackMontage
		&& CharacterState != ECharacterState::ECS_Unarmed
		&& CharacterActionState == EActionState::EAS_Unoccupied
		&& EquippedWeapon->WeaponState == EWeaponState::EWS_InHand;
}

void APlayerCharacter::Sheathe() 
{
	// Get the animation montage
	UAnimInstance* CurrentInstance = GetMesh()->GetAnimInstance();
	// Early return if no equipped weapon or the current animation instance doesn't exist.
	if (!CurrentInstance || !EquippedWeapon)
	{
		return;
	}
	// If the equipped weapon is on our back and we are not occupied
	if ((EquippedWeapon->WeaponState == EWeaponState::EWS_OnBack) && CanSheathe())
	{
		CharacterActionState = EActionState::EAS_Occupied;
		CurrentInstance->Montage_Play(EquipMontage);
		CurrentInstance->Montage_JumpToSection(FName("DrawTwoHanded"));
	
		// In Blueprint, call Unsheathe Weapon at appropriate notify
		
	}
	// If the equipped weapon is in our hand and we are not occupied
	else if ((EquippedWeapon->WeaponState == EWeaponState::EWS_InHand) && CanSheathe()) 
	{
		CharacterActionState = EActionState::EAS_Occupied;
		CurrentInstance->Montage_Play(EquipMontage);
		CurrentInstance->Montage_JumpToSection(FName("SheatheTwoHanded"));
		// In Blueprint, set the weapon state to on back and set character action state back to unoccupied on notify end.
		// Call SheatheWeapon() in Blueprint at appropriate notify
	}
	
		
}

// Can sheathe weapon if equip montage is set && character is unoccupied and armed

bool APlayerCharacter::CanSheathe() 
{
	return EquipMontage
		&& (CharacterActionState == EActionState::EAS_Unoccupied)
		&& EquippedWeapon;
}

// Call these functions after notify end
void APlayerCharacter::SheatheWeapon()
{
	if (EquippedWeapon) 
	{
		EquippedWeapon->Unequip(GetMesh(), FName("SpineSocket"));
	}
}

// Call after notify end
void APlayerCharacter::UnsheatheWeapon()
{
	if (EquippedWeapon) 
	{
		EquippedWeapon->Equip(GetMesh(), FName("SpineSocket"), this, this);
	}
}

void APlayerCharacter::GetHit(const FVector& ImpactPoint)
{
	// TODO
}
