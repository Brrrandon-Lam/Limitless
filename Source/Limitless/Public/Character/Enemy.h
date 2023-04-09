// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter.h"
#include "Interfaces/HitInterface.h"
#include "Components/Attributes.h"
#include "CharacterState.h"
#include "Enemy.generated.h"

class UAnimMontage;
class UHealthbarComponent;
class AAIController;
class UPawnSensingComponent;

UCLASS()
class LIMITLESS_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetHit(const FVector& ImpactPoint) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	bool WithinRange(AActor* Target, double Range);

	void PatrolTimerEnd();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Ensure the initial patrol point is set on the instance!
	UPROPERTY(EditInstanceOnly, Category = "AI Movement")
	AActor* PatrolPoint;

	// Ensure all of the actor's patrol points are set on the instance!
	UPROPERTY(EditInstanceOnly, Category = "AI Movement")
	TArray<AActor*> PatrolPath;

	UFUNCTION()
	void MoveToTarget(AActor* Target);
	
	UFUNCTION()
	void SelectPatrolPoint();

	UFUNCTION()
	void PatrolMode();
	
	// Radial Values
	UPROPERTY(EditAnywhere)
	double PatrolRadius = 200.0;

	UPROPERTY(EditAnywhere)
	double CombatRadius = 500.0;

	UPROPERTY(EditAnywhere)
	double AttackRadius = 150.0;

	UFUNCTION()
	void PawnSeen(APawn* SensedPawn);

private:

	UPROPERTY(VisibleAnywhere)
	UHealthbarComponent* HealthbarComponent;

	UPROPERTY()
	AAIController* EnemyController;

	UPROPERTY()
	int CurrentPatrolIndex = 0;

	UPROPERTY(EditInstanceOnly, Category = "AI Movement")
	float PatrolWaitMin = 5.0f;
	UPROPERTY(EditInstanceOnly, Category = "AI Movement")
	float PatrolWaitMax = 10.0f;

	FTimerHandle PatrolTimer;

	UPROPERTY()
	EEnemyState EnemyState = EEnemyState::EES_Patrolling;

	UPROPERTY(VisibleAnywhere)
	UPawnSensingComponent* PawnSensingComponent;

	UPROPERTY(VisibleInstanceOnly)
	APawn* CombatTarget;

	UFUNCTION()
	void CombatMode();


};
