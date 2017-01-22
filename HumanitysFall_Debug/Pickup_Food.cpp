// Fill out your copyright notice in the Description page of Project Settings.

#include "HumanitysFall_Debug.h"
#include "HumanitysFall_DebugCharacter.h"
#include "Pickup_Food.h"

APickup_Food::APickup_Food()
{
	amountOfFood = 0;
}

void APickup_Food::Interact_Implementation()
{
	AHumanitysFall_DebugCharacter* character = Cast<AHumanitysFall_DebugCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	character->UpdateFood(amountOfFood);
	Destroy();
}
