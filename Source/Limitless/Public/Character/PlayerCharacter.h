// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "GameFramework/Character.h"
#include "Character/BaseCharacter.h"
#include "CharacterState.h"
#include "PlayerCharacter.generated.h"

// Forward Declarations
class UCameraComponent;
class USpringArmComponent;
class UGroomComponent;
class AItem;
class UAnimMontage;

UCLASS()
class LIMITLESS_API APlayerCharacter : public ABaseCharacter
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
	void Attack() override;
	void HeavyAttack();
	void Sheathe();

	virtual void GetHit(const FVector& ImpactPoint) override;

	UFUNCTION(BlueprintCallable)
	void SheatheWeapon();
	UFUNCTION(BlueprintCallable)
	void UnsheatheWeapon();

	// Check our attack conditions
	bool CanAttack();
	// Check whether we can sheathe/unsheathe
	bool CanSheathe();

	UPROPERTY(BlueprintReadWrite)
	ECharacterState CharacterState;

	UPROPERTY(BlueprintReadWrite)
	EActionState CharacterActionState;

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

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* EquipMontage;

};
