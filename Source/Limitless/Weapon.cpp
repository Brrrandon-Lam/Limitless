// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "HitInterface.h"

AWeapon::AWeapon()
{
	WeaponBoxCollider = CreateDefaultSubobject<UBoxComponent>(FName("Weapon Hitbox"));
	WeaponBoxCollider->SetupAttachment(GetRootComponent());
	// No collisions unless we are attacking.
	WeaponBoxCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponBoxCollider->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	WeaponBoxCollider->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	WeaponBoxCollider->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	
	BoxTraceStart = CreateDefaultSubobject<USceneComponent>(FName("Trace Start"));
	BoxTraceStart->SetupAttachment(GetRootComponent());
	
	BoxTraceEnd = CreateDefaultSubobject<USceneComponent>(FName("Trace End"));
	BoxTraceEnd->SetupAttachment(GetRootComponent());

	WeaponBoxCollider->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnWeaponHitboxBeginOverlap);

}

// Attach to the right hand and set the state
void AWeapon::Equip(USceneComponent* InParent, FName SocketName)
{
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	ItemMesh->AttachToComponent(InParent, TransformRules, FName(TEXT("RightHandSocket")));
	WeaponState = EWeaponState::EWS_InHand;
}

// Attach to the back and set the state.
void AWeapon::Unequip(USceneComponent* InParent, FName SocketName)
{
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	ItemMesh->AttachToComponent(InParent, TransformRules, FName(TEXT("SpineSocket")));
	WeaponState = EWeaponState::EWS_OnBack;

}

void AWeapon::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

}

void AWeapon::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnSphereEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

void AWeapon::OnWeaponHitboxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	// When we begin overlap, do a box trace.
	FVector StartLocation = BoxTraceStart->GetComponentLocation();
	FVector EndLocation = BoxTraceEnd->GetComponentLocation();
	FVector HalfSize = FVector(2.5, 2.5, 2.5);
	TArray<AActor*> ActorsToIgnore{ this };
	FHitResult HitResult;

	bool bSuccess = UKismetSystemLibrary::BoxTraceSingle(this, StartLocation,EndLocation,HalfSize,WeaponBoxCollider->GetComponentRotation(),ETraceTypeQuery::TraceTypeQuery1,false,ActorsToIgnore,EDrawDebugTrace::ForDuration,HitResult,true,FLinearColor::Red,FLinearColor::Green,2.0f);

	if (bSuccess)
	{
		// Try to cast the actor hit into a HitInterface
		IHitInterface* HitObject = Cast<IHitInterface>(HitResult.GetActor());
		// If the object hit can be casted to Hit Interface
		if (HitObject)
		{
			HitObject->GetHit(HitResult.ImpactPoint);
		}
		UE_LOG(LogTemp, Warning, TEXT("HIT OBJECT"));
		
	}

}

