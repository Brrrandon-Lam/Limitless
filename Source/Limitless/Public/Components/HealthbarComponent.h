// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "HealthbarComponent.generated.h"

class UHealthbar;

/**
 * 
 */
UCLASS()
class LIMITLESS_API UHealthbarComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	void SetHealthPercentage(float Value);

private:
	UPROPERTY()
	UHealthbar* HealthbarWidget;

};
