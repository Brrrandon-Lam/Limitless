// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterState.h"
#include "PlayerCharacter.generated.h"

// Forward Declarations
class UCameraComponent;
class USpringArmComponent;
class UGroomComponent;
class AItem;
class UAnimMontage;
class AWeapon;

UCLASS()
class LIMITLESS_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// Actions
	void EquipButtonPressed();
	FORCEINLINE void SetOverlappingItem(AItem* Item) {	OverlappingItem = Item; }

	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Movement Functions
	void MoveForward(float Value);
	void MoveRight(float Value);
	void LookUp(float Value);
	void LookRight(float Value);
	void Attack();
	void HeavyAttack();
	void Sheathe();

	UFUNCTION(BlueprintCallable)
	void SheatheWeapon();
	UFUNCTION(BlueprintCallable)
	void UnsheatheWeapon();

	// Check our attack conditions
	bool CanAttack();
	// Check whether we can sheathe/unsheathe
	bool CanSheathe();

	// Toggle box collider of the currently equipped weapon.
	UFUNCTION(BlueprintCallable)
	void ToggleWeaponCollision(ECollisionEnabled::Type CollisionsToggle);

	UPROPERTY(BlueprintReadWrite)
	ECharacterState CharacterState;

	UPROPERTY(BlueprintReadWrite)
	EActionState CharacterActionState;

	UPROPERTY(BlueprintReadOnly)
	AWeapon* EquippedWeapon;



private:

	UPROPERTY(VisibleInstanceOnly)
	AItem* OverlappingItem;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* CameraBoom;
	
	UPROPERTY(VisibleAnywhere)
	UGroomComponent* Hair;

	UPROPERTY(VisibleAnywhere)
	UGroomComponent* Eyebrows;

	/* Montages */
	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* EquipMontage;

};
