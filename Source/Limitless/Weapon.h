// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Weapon.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	EWS_OnGround UMETA(DisplayName = "OnGround"),
	EWS_InHand UMETA(DisplayName = "InHand"),
	EWS_OnBack UMETA(DisplayName = "OnBack")
};

UCLASS()
class LIMITLESS_API AWeapon : public AItem
{
	GENERATED_BODY()

public:
	// Need the character mesh component
	void Equip(USceneComponent* InParent, FName SocketName);
	void Unequip(USceneComponent* InParent, FName SocketName);

	UPROPERTY(BlueprintReadWrite)
	EWeaponState WeaponState = EWeaponState::EWS_OnGround;

protected:
	virtual void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
	
};
