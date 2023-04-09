// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/DestructibleObject.h"
#include "GeometryCollection/GeometryCollectionComponent.h"

// Sets default values
ADestructibleObject::ADestructibleObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Initialize a geometry collection component as the root
	GeometryCollection = CreateDefaultSubobject<UGeometryCollectionComponent>("Geometry Collection");
	SetRootComponent(GeometryCollection);
	GeometryCollection->SetCollisionObjectType(ECollisionChannel::ECC_Destructible);
	GeometryCollection->SetGenerateOverlapEvents(true);
	GeometryCollection->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

}

// Called when the game starts or when spawned
void ADestructibleObject::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADestructibleObject::GetHit(const FVector& ImpactPoint)
{
	// Play a breaking sound at location
}

