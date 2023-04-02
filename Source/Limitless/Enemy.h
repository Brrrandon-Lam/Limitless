// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HitInterface.h"
#include "Enemy.generated.h"

class UAnimMontage;
class UAttributes;
class UHealthbarComponent;
class AAIController;

UCLASS()
class LIMITLESS_API AEnemy : public ACharacter, public IHitInterface
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

	double CalculateImpactAngle(const FVector& ImpactPoint);

	void PlayHitReactionMontage(const FName Section);
	void PlayHitDirectionAnimation(const double angle);
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	bool WithinRange(AActor* Target, double Range);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* HitReactionMontage;

	// Ensure the initial patrol point is set on the instance!
	UPROPERTY(EditInstanceOnly, Category = "AI Movement")
	AActor* PatrolPoint;

	UPROPERTY(VisibleAnywhere, Category = "AI Movement")
	AActor* NextPatrolPoint;

	// Ensure all of the actor's patrol points are set on the instance!
	UPROPERTY(EditInstanceOnly, Category = "AI Movement")
	TArray<AActor*> PatrolPath;

	void MoveToPatrolPoint(AActor* Target);
	void SelectPatrolPoint();
	// Variable that holds the min sq distance to a ctrl point
	UPROPERTY(EditAnywhere)
	double PatrolRadius = 200.0;

private:

	UPROPERTY(VisibleAnywhere)
	UHealthbarComponent* HealthbarComponent;

	UPROPERTY(VisibleAnywhere)
	UAttributes* AttributesComponent;

	UPROPERTY()
	AAIController* EnemyController;

	UPROPERTY()
	int CurrentPatrolIndex = 0;


};
