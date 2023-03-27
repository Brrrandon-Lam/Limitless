// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DrawDebugHelpers.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);

}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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