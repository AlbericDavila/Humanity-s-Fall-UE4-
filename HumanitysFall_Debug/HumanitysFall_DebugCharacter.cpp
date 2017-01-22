// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "HumanitysFall_Debug.h"
#include "Kismet/HeadMountedDisplayFunctionLibrary.h"
#include "HumanitysFall_DebugCharacter.h"
#include "HumanitysFall_DebugGameMode.h"

//////////////////////////////////////////////////////////////////////////
// AHumanitysFall_DebugCharacter


AHumanitysFall_DebugCharacter::AHumanitysFall_DebugCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	/*// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	*/

	reach = 250.0f;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

void AHumanitysFall_DebugCharacter::BeginPlay()
{
	Super::BeginPlay();

	Inventory.SetNum(4);
	currentInteractable = nullptr;
}

void AHumanitysFall_DebugCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckForInteractables();
}

//////////////////////////////////////////////////////////////////////////
// Input

void AHumanitysFall_DebugCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	
	//**
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AHumanitysFall_DebugCharacter::Interact);
	PlayerInputComponent->BindAction("ToggleInventory", IE_Pressed, this, &AHumanitysFall_DebugCharacter::ToggleInventory);
	//**

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AHumanitysFall_DebugCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AHumanitysFall_DebugCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AHumanitysFall_DebugCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AHumanitysFall_DebugCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AHumanitysFall_DebugCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AHumanitysFall_DebugCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AHumanitysFall_DebugCharacter::OnResetVR);
}

void AHumanitysFall_DebugCharacter::CalculateDead()
{
	if (health <= 0)
		isDead = true;
	else
		isDead = false;
}

void AHumanitysFall_DebugCharacter::CalculateHealth(float delta)
{
	health += delta;
	CalculateDead();
}

#if WITH_EDITOR
void AHumanitysFall_DebugCharacter::PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent)
{
	isDead = false;
	// Was changed from 0 to 100, might need testing.
	health = 0;
	Super::PostEditChangeProperty(PropertyChangedEvent);
	CalculateDead();
}
#endif

void AHumanitysFall_DebugCharacter::UpdateFood(int32 amount)
{
	food += amount;
}

bool AHumanitysFall_DebugCharacter::AddItemToInventory(APickup * item)
{
	if (item != NULL)
	{
		// Find the first available slot (slot with nullptr).
		const int32 availableSlot = Inventory.Find(nullptr);

		if (availableSlot != INDEX_NONE)
		{
			Inventory[availableSlot] = item;
			return true;
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Inventory Full!"));
			return false;
		}
	}
	else return false;
}

// Gets the thumbnail for an item in a slot inventory.
UTexture2D * AHumanitysFall_DebugCharacter::GetThumbnailAtInventorySlot(int32 slot)
{
	if (Inventory[slot] != NULL)
	{
		return Inventory[slot]->pickupThumbnail;
	}
	else return nullptr;
}

// Return the item's name from inventory slot, if empty display "None".
FString AHumanitysFall_DebugCharacter::GetItemNameAtInventorySlot(int32 slot)
{
	if (Inventory[slot] != NULL)
	{
		return Inventory[slot]->itemName;
	}
	return FString("None");
}

void AHumanitysFall_DebugCharacter::UseItemAtInventorySlot(int32 slot)
{
	if (Inventory[slot] != NULL)
	{
		Inventory[slot]->Use_Implementation();  // Use item at inventory slot throught item's function Use_Implementation.
		Inventory[slot] = NULL;					// Delete item after it's used.
	}
}

void AHumanitysFall_DebugCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AHumanitysFall_DebugCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	// jump, but only on the first touch
	if (FingerIndex == ETouchIndex::Touch1)
	{
		Jump();
	}
}

void AHumanitysFall_DebugCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	if (FingerIndex == ETouchIndex::Touch1)
	{
		StopJumping();
	}
}

void AHumanitysFall_DebugCharacter::ToggleInventory()
{
	// Check player's HUD state, if inventory is open then close, else open it.
	GLog->Log("Toggled Inventory");
	AHumanitysFall_DebugGameMode* gameMode = Cast<AHumanitysFall_DebugGameMode>(GetWorld()->GetAuthGameMode());

	if (gameMode->GetHUDState() == gameMode->HS_Ingame)
	{
		GLog->Log("Had In Game HUD");
		gameMode->ChangeHUDState(gameMode->HS_Inventory);
	}
	else
	{
		GLog->Log("Did not had In Game HUD");
		gameMode->ChangeHUDState(gameMode->HS_Ingame);
	}
}

void AHumanitysFall_DebugCharacter::Interact()
{
	if (currentInteractable != nullptr)
	{
		currentInteractable->Interact_Implementation();
	}
}

void AHumanitysFall_DebugCharacter::CheckForInteractables()
{
	// To linetrace, get the start and end traces
	FVector startTrace = CameraBoom->GetComponentLocation();
	FVector endTrace = (CameraBoom->GetForwardVector() * reach) + startTrace;

	// Declare a hit result to store the raycast hit in
	FHitResult hitResult;

	// Initialize the query params - ignore the actor.
	FCollisionQueryParams CQP;
	CQP.AddIgnoredActor(this);

	// Cast the linetrace.
	GetWorld()->LineTraceSingleByChannel(hitResult, startTrace, endTrace, ECC_WorldDynamic, CQP);

	AInteractable* potentialInteractable = Cast<AInteractable>(hitResult.GetActor());

	if (potentialInteractable == NULL)
	{
		helpText = FString("");
		currentInteractable = nullptr;
		return;
	}
	else
	{
		currentInteractable = potentialInteractable;
		helpText = potentialInteractable->InteractableHelpText;
	}

}

void AHumanitysFall_DebugCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AHumanitysFall_DebugCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AHumanitysFall_DebugCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AHumanitysFall_DebugCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}
