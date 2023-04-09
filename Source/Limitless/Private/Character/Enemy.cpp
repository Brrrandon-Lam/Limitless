// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DrawDebugHelpers.h"
#include "Components/WidgetComponent.h"
#include "Components/HealthbarComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"
#include "Perception/PawnSensingComponent.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);

	HealthbarComponent = CreateDefaultSubobject<UHealthbarComponent>(TEXT("Health Bar"));
	HealthbarComponent->SetupAttachment(GetRootComponent());
	
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Pawn Sensing Component"));
	PawnSensingComponent->SightRadius = 4000;
	PawnSensingComponent->SetPeripheralVisionAngle(45.0f);

	// Don't use controller rotation, orient character to movement.
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	
	GetCharacterMovement()->bOrientRotationToMovement = true;

	CurrentPatrolIndex = 0;

	CombatTarget = nullptr;
	
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	if (HealthbarComponent)
	{
		HealthbarComponent->SetHealthPercentage(1.0f);
	}
	EnemyController = Cast<AAIController>(GetController());
	if (PatrolPoint && EnemyController)
	{
		MoveToTarget(PatrolPoint);
	}
	if (PawnSensingComponent)
	{
		PawnSensingComponent->OnSeePawn.AddDynamic(this, &AEnemy::PawnSeen);
	}
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Combat Mode
	if (EnemyState != EEnemyState::EES_Patrolling)
	{
		CombatMode();
	}

	// Patrol Mode
	else 
	{
		PatrolMode();
		
	}
}

void AEnemy::PatrolMode()
{
	// If we're near a patrol point
	if (WithinRange(PatrolPoint, PatrolRadius))
	{
		// Select a patrol point
		SelectPatrolPoint();
		// Move to the patrol point
		GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemy::PatrolTimerEnd, FMath::FRandRange(PatrolWaitMin, PatrolWaitMax));
	}
}

void AEnemy::PatrolTimerEnd()
{
	MoveToTarget(PatrolPoint);
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::GetHit(const FVector& ImpactPoint)
{
	// Calculate the angle
	double angle = CalculateImpactAngle(ImpactPoint);
	// Play directional damage animation
	PlayHitDirectionAnimation(angle);

	// TODO: Calculate damage taken

}

// TakeDamage implementation for the Enemy class

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	// Check that Attributes and Healthbar Widget exist
	if (Attributes && HealthbarComponent)
	{
		// Send the amount of damage received.
		Attributes->ReceiveDamage(DamageAmount);
		// Update health percent.
		HealthbarComponent->SetHealthPercentage(Attributes->GetHealthPercentage());

	}
	// If the health drops to 0 or lower, play a death animation
	if (Attributes->GetCurrentHealth() == 0) {
		UE_LOG(LogTemp, Warning, TEXT("ENEMY DIED!"));
	}

	// Aggro to whatever damages the enemy
	CombatTarget = EventInstigator->GetPawn();
	EnemyState = EEnemyState::EES_Chasing;
	MoveToTarget(CombatTarget);

	// Return the amount of damage applied.
	return DamageAmount;
}

// Given a target, move to it.
void AEnemy::MoveToTarget(AActor* Target)
{
	// We cannot execute this code without both a target and an AI controller.
	if (!Target || !EnemyController) { return; }
	
	// Create a move request and move to the target.
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(Target);
	MoveRequest.SetAcceptanceRadius(10.0f);
	EnemyController->MoveTo(MoveRequest);
	
}

void AEnemy::SelectPatrolPoint()
{
	if (CurrentPatrolIndex >= PatrolPath.Num() - 1)
	{
		CurrentPatrolIndex = 0;
		PatrolPoint = PatrolPath[0];
	}
	else
	{
		CurrentPatrolIndex += 1;
		PatrolPoint = PatrolPath[CurrentPatrolIndex];
	}
}

bool AEnemy::WithinRange(AActor* Target, double Range)
{
	if (!Target)
	{
		return false;
	}
	const double DistToTarget = (Target->GetActorLocation() - GetActorLocation()).Size();
	return DistToTarget <= Range;
}

// Delegate function callback for Pawn Sensing Component, sets the pawn's mode to chasing only if the enemy is in patrolling mode and if the enemy detected a player.
void AEnemy::PawnSeen(APawn* SensedPawn)
{
	// Only move to target if we're not alrady chasing.
	if (EnemyState == EEnemyState::EES_Chasing)
	{ 
		return;
	}

	if(SensedPawn->ActorHasTag(FName("Player")))
	{
		GetWorldTimerManager().ClearTimer(PatrolTimer);
		GetCharacterMovement()->MaxWalkSpeed = 350.0f;
		CombatTarget = SensedPawn;
		// If attacking, pawn seen doesn't need to do anything else.
		if (EnemyState != EEnemyState::EES_Attacking)
		{
			EnemyState = EEnemyState::EES_Chasing;
			MoveToTarget(CombatTarget);
		}
		
	}
}

void AEnemy::CombatMode()
{
	DrawDebugSphere(GetWorld(), GetActorLocation(), CombatRadius, 15, FColor::Red);
	// Return to patrol mode if we exit a combat radius
	if (!WithinRange(CombatTarget, CombatRadius))
	{
		UE_LOG(LogTemp, Warning, TEXT("Return to Patrolling"));
		// Unset combat target, set state to patrolling, move to the last stored patrol point.
		CombatTarget = nullptr;
		EnemyState = EEnemyState::EES_Patrolling;
		GetCharacterMovement()->MaxWalkSpeed = 150.0f;
		MoveToTarget(PatrolPoint);
		UE_LOG(LogTemp, Warning, TEXT("Chasing - Combat Mode"));
	}
	// If we're not already chasing and we're not in attack range
	else if (!WithinRange(CombatTarget, AttackRadius) && EnemyState != EEnemyState::EES_Chasing)
	{
		// Continue chasing the character until we enter attack range.
		EnemyState = EEnemyState::EES_Chasing;
		GetCharacterMovement()->MaxWalkSpeed = 350.0f;
		MoveToTarget(CombatTarget);
	}
	else if (WithinRange(CombatTarget, AttackRadius) && EnemyState != EEnemyState::EES_Attacking)
	{
		// Attack the character
		EnemyState = EEnemyState::EES_Attacking;
		UE_LOG(LogTemp, Warning, TEXT("Attacking!"));
		// Play an attack montage
	}
}
