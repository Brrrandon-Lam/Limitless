// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DrawDebugHelpers.h"
#include "Components/Attributes.h"
#include "Components/WidgetComponent.h"
#include "HUD/HealthbarComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);

	// Initialize Enemy Components
	AttributesComponent = CreateDefaultSubobject<UAttributes>(TEXT("Attributes"));

	HealthbarComponent = CreateDefaultSubobject<UHealthbarComponent>(TEXT("Health Bar"));
	HealthbarComponent->SetupAttachment(GetRootComponent());

	// Don't use controller rotation, orient character to movement.
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	
	GetCharacterMovement()->bOrientRotationToMovement = true;

	CurrentPatrolIndex = 0;
	
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
		MoveToPatrolPoint(PatrolPoint);
	}
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// Only update and move when we encroach on a patrol point.
	if (WithinRange(PatrolPoint, PatrolRadius))
	{
		SelectPatrolPoint();
		MoveToPatrolPoint(PatrolPoint);
	}
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

double AEnemy::CalculateImpactAngle(const FVector& ImpactPoint)
{
	// Calculate the dot product between the actor's forward vector and the hit vector to get an angle.;
	FVector ForwardVector = GetActorForwardVector();
	FVector Impact(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	FVector ImpactVector = (Impact - GetActorLocation()).GetSafeNormal();

	// Dot product returns cos(theta), return arccos(theta) and convert to degrees from the dot product.
	double angle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(ForwardVector, ImpactVector)));

	// Using the cross product between the forward vector and the hit vector, determine whether the angle is positive or negative.
	FVector ImpactCrossProduct = FVector::CrossProduct(ForwardVector, ImpactVector);

	// Is the orthogonal vector facing up or down?
	if (ImpactCrossProduct.Z < 0)
	{
		angle *= -1.0;
	}

	return angle;

	/* -- DEBUGGING  
	UE_LOG(LogTemp, Warning, TEXT("Hit Angle: %f"), angle);
	DrawDebugSphere(GetWorld(), ImpactPoint, 5.0f, 10, FColor::Green, true);
	DrawDebugDirectionalArrow(GetWorld(), GetActorLocation(), GetActorLocation() + ForwardVector * 60.f, 5.0f, FColor::Red, true);
	DrawDebugDirectionalArrow(GetWorld(), GetActorLocation(), GetActorLocation() + ImpactVector * 60.f, 5.0f, FColor::Green, true);
	DrawDebugDirectionalArrow(GetWorld(), GetActorLocation(), GetActorLocation() + ImpactCrossProduct * 60.f, 5.0f, FColor::Blue, true); -- */
}

void AEnemy::PlayHitReactionMontage(const FName Section)
{
	UAnimInstance* CurrentInstance = GetMesh()->GetAnimInstance();
	if (CurrentInstance && HitReactionMontage)
	{
		CurrentInstance->Montage_Play(HitReactionMontage);
		CurrentInstance->Montage_JumpToSection(Section, HitReactionMontage);
	}
}

void AEnemy::PlayHitDirectionAnimation(const double angle)
{
	if (!GEngine)
	{
		return;
	}
	// Print Front, Back, Left, Right depending on the angle.
	// Front: -45 <= x <= 45
	if (angle >= -45 && angle <= 45) 
	{
		PlayHitReactionMontage(FName("FrontHitReaction"));
	}
	// Back: Angle is greater than 135 or less than -135
	else if (angle >= 135 || angle <= -135) 
	{
		PlayHitReactionMontage(FName("BackHitReaction"));
	}
	// Left: -135 < x < -45
	else if (angle > -135 && angle < -45) 
	{
		PlayHitReactionMontage(FName("LeftHitReaction"));
	}
	// Right: 45 < x < 135
	else if (angle > 45 && angle < 135) 
	{
		PlayHitReactionMontage(FName("RightHitReaction"));
	}
	else 
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.0f, FColor::Cyan, FString("ERROR"));
	}
}

// TakeDamage implementation for the Enemy class

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	// Check that Attributes and Healthbar Widget exist
	if (AttributesComponent && HealthbarComponent)
	{
		// Send the amount of damage received.
		AttributesComponent->ReceiveDamage(DamageAmount);
		// Update health percent.
		HealthbarComponent->SetHealthPercentage(AttributesComponent->GetHealthPercentage());

	}
	// If the health drops to 0 or lower, play a death animation
	if (AttributesComponent->GetCurrentHealth() == 0) {
		UE_LOG(LogTemp, Warning, TEXT("ENEMY DIED!"));
	}

	// Return the amount of damage applied.
	return DamageAmount;
}

// Given a target, move to it.
void AEnemy::MoveToPatrolPoint(AActor* Target)
{
	// We cannot execute this code without both a target and an AI controller.
	if (!Target || !EnemyController) { return; }
	
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(Target);
	MoveRequest.SetAcceptanceRadius(20.0f);
	GetCharacterMovement()->MaxWalkSpeed = 150.0f;
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

// Potentially abstract to a math header.
bool AEnemy::WithinRange(AActor* Target, double Range)
{
	const double DistToTarget = (Target->GetActorLocation() - GetActorLocation()).Size();
	return DistToTarget <= Range;
}