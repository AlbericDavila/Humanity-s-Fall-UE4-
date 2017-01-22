// Fill out your copyright notice in the Description page of Project Settings.

#include "HumanitysFall_Debug.h"
#include "HumanitysFall_DebugCharacter.h"
#include "Pickup.h"

APickup::APickup()
{
	//Set up mesh, item name, item value.
	InteractableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
	InteractableMesh->SetSimulatePhysics(true);

	// Give the item a name.
	itemName = FString("Enter item name.");
	InteractableHelpText = FString("Press E to pick up");
	value = 0;
}

void APickup::BeginPlay()
{
	InteractableHelpText = FString::Printf(TEXT("Press E to pick up"));
}

// What the item will be doing when used.
void APickup::Interact_Implementation()
{
	// Create reference to character.
	AHumanitysFall_DebugCharacter* character = Cast<AHumanitysFall_DebugCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));

	if(character->AddItemToInventory(this))
		OnPickedUp();
	
}

void APickup::OnPickedUp()
{
	InteractableMesh->SetVisibility(false);
	InteractableMesh->SetSimulatePhysics(false);
	InteractableMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}