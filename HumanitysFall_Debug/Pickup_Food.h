// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Pickup.h"
#include "Pickup_Food.generated.h"

/**
 * 
 */
UCLASS()
class HUMANITYSFALL_DEBUG_API APickup_Food : public APickup
{
	GENERATED_BODY()

public:

	APickup_Food();

	virtual void Interact_Implementation() override;
	
	UPROPERTY(Editanywhere, Category = "Food Properties")
	int32 amountOfFood;
	
	
	
};
