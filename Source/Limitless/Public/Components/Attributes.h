// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Attributes.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LIMITLESS_API UAttributes : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAttributes();

	FORCEINLINE void SetCurrentHealth(float Value) { CurrentHealth = Value; }
	FORCEINLINE float GetCurrentHealth() const { return CurrentHealth; }
	FORCEINLINE float GetHealthPercentage() const { return (CurrentHealth / MaxHealth); }
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void ReceiveDamage(float Value);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


private:
	
	UPROPERTY(EditAnywhere)
	float MaxHealth;

	UPROPERTY(VisibleAnywhere)
	float CurrentHealth;
		
};
