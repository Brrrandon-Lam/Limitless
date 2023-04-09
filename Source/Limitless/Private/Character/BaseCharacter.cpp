// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BaseCharacter.h"
#include "Components/Attributes.h"
#include "Items/Weapon.h"
#include "Components/BoxComponent.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Construct all characters with an Attributes component
	Attributes = CreateDefaultSubobject<UAttributes>(TEXT("Attributes"));
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseCharacter::Attack()
{
}

void ABaseCharacter::Death()
{
}

void ABaseCharacter::GetHit(const FVector& ImpactPoint)
{
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Get the box collider component of the equipped weapon, set collisions enabled.
void ABaseCharacter::ToggleWeaponCollision(ECollisionEnabled::Type CollisionsToggle)
{
	if (EquippedWeapon)
	{
		EquippedWeapon->GetWeaponBoxCollider()->SetCollisionEnabled(CollisionsToggle);
	}
}

void ABaseCharacter::PlayHitReactionMontage(const FName Section)
{
	UAnimInstance* CurrentInstance = GetMesh()->GetAnimInstance();
	if (CurrentInstance && HitReactionMontage)
	{
		CurrentInstance->Montage_Play(HitReactionMontage);
		CurrentInstance->Montage_JumpToSection(Section, HitReactionMontage);
	}
}

double ABaseCharacter::CalculateImpactAngle(const FVector& ImpactPoint)
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
}

void ABaseCharacter::PlayHitDirectionAnimation(const double angle)
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