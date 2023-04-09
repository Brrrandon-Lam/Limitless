// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Healthbar.generated.h"

class UProgressBar;

/**
 * 
 */
UCLASS()
class LIMITLESS_API UHealthbar : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// Bind the progress bar, Healthbar, to the Healthbar component on our WBP_Healthbar widget.
	UPROPERTY(meta = (BindWidget))
	UProgressBar* Healthbar;

};
