// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/HealthbarComponent.h"
#include "Components/Healthbar.h"
#include "Components/ProgressBar.h"

void UHealthbarComponent::SetHealthPercentage(float Value)
{
	// Whenever this function is called, attempt to grab the Healthbar
	if (!HealthbarWidget) {
		HealthbarWidget = Cast<UHealthbar>(GetUserWidgetObject());
	}
	// Check that the widget and the Healthbar are set.
	if (HealthbarWidget && HealthbarWidget->Healthbar)
	{
		// Set the current value of the progress bar.
		HealthbarWidget->Healthbar->SetPercent(Value);
	}
}
