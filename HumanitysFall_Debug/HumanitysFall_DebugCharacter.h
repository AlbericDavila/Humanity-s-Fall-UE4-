// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "Interactable.h"
#include "Pickup.h"
#include "HumanitysFall_DebugCharacter.generated.h"

UCLASS(config=Game)
class AHumanitysFall_DebugCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;*/
	
public:

	// Make health property
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Character")
	float health = 100;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Character")
	bool isDead = false;

	// Calculate death function
	virtual void CalculateDead();

	UFUNCTION(BlueprintCallable, Category = "Character")
	virtual void CalculateHealth(float delta);

#if WITH_EDITOR
	 //Editor-centric code for changing properties
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	AHumanitysFall_DebugCharacter();

	virtual void BeginPlay();

	virtual void Tick(float DeltaTime) override;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	// Player help text.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	FString helpText;

	// Amount of gold.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	int32 food;

	/* Updates the coins
	@param amount - this amount updates the coins, can be positive or negative.
	*/
	UFUNCTION(BlueprintCallable, Category = "Inventory Functions")
	void UpdateFood(int32 amount);

	// Add item to inventory.
	UFUNCTION(BlueprintPure, Category = "Inventory Functions")
	bool AddItemToInventory(APickup* item);

	// Gets thumbnaol for an inventory slot
	UFUNCTION(BlueprintPure, Category = "Inventory Functions")
	UTexture2D* GetThumbnailAtInventorySlot(int32 slot);

	// Gets item name for inventory slot.
	UFUNCTION(BlueprintPure, Category = "Inventory Functions")
	FString GetItemNameAtInventorySlot(int32 slot);

	// Uses the item at inventory slot
	UFUNCTION(BlueprintCallable, Category = "Inventory Functions")
	void UseItemAtInventorySlot(int32 slot);

protected:

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

private:

	// Player's reach to interact with an item.
	float reach;

	// Bring up inventory.
	void ToggleInventory();

	// Interact with interactable.
	void Interact();

	// Check for interactable item in front of the player using linetrace.
	void CheckForInteractables();

	// Interactable player is looking at
	AInteractable* currentInteractable;

	// The player inventory as a TArray of pickup items.
	UPROPERTY(EditAnywhere)
	TArray<APickup*> Inventory;

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject*/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject*/
	//FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

