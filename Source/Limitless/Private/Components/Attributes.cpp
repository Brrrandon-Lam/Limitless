// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Attributes.h"

// Sets default values for this component's properties
UAttributes::UAttributes()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	MaxHealth = 100.0f;
	CurrentHealth = MaxHealth;
}


// Called when the game starts
void UAttributes::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UAttributes::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

// Function that modifies CurrentHealth, preventing it from exceeding max health and dropping below 0 using FMath::Clamp().
void UAttributes::ReceiveDamage(float DamageReceived)
{
	CurrentHealth = FMath::Clamp(CurrentHealth - DamageReceived, 0.f, MaxHealth);
}

