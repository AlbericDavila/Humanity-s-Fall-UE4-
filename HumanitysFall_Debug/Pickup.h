// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Interactable.h"
#include "Pickup.generated.h"

/**
 * 
 */
UCLASS()
class HUMANITYSFALL_DEBUG_API APickup : public AInteractable
{
	GENERATED_BODY()

public:
	APickup();

	virtual void BeginPlay() override;

	virtual void Interact_Implementation() override;

	UFUNCTION(BlueprintNativeEvent)
	void Use();
	virtual void Use_Implementation();

	UPROPERTY(EditAnywhere, Category = "Pickup Properties")
	UTexture2D* pickupThumbnail;

	// Name of the item.
	UPROPERTY(EditAnywhere, Category = "Pickup Properties")
	FString itemName;

	// GIves the item (such as coins) a value.
	UPROPERTY(EditAnywhere, Category = "Pickup Properties")
		int32 value;


	// When Item is picked up.
	void OnPickedUp();
	
};
