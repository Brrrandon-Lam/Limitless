// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "BaseCharacter.generated.h"

class AWeapon;
class UAttributes;
class UAnimMontage;

UCLASS()
class LIMITLESS_API ABaseCharacter : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Toggle box collider of the currently equipped weapon.
	UFUNCTION(BlueprintCallable)
	void ToggleWeaponCollision(ECollisionEnabled::Type CollisionsToggle);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Attack();
	void PlayHitReactionMontage(const FName Section);
	void PlayHitDirectionAnimation(const double angle);
	double CalculateImpactAngle(const FVector& ImpactPoint);
	virtual void Death();
	virtual void GetHit(const FVector& ImpactPoint) override;
	

	// virtual void AttackEnd();

	UPROPERTY(BlueprintReadOnly)
	AWeapon* EquippedWeapon;

	UPROPERTY(VisibleAnywhere)
	UAttributes* Attributes;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* HitReactionMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* DeathMontage;


};
