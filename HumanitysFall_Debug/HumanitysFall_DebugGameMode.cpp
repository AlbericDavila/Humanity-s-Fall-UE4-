// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "HumanitysFall_Debug.h"
#include "HumanitysFall_DebugCharacter.h"
#include "HumanitysFall_DebugGameMode.h"
#include "CharacterHUD.h"
#include "Runtime/UMG/Public/UMG.h"
#include "Runtime/UMG/Public/UMGStyle.h"
#include "Runtime/UMG/Public/Slate/SObjectWidget.h"
#include "Runtime/UMG/Public/IUMGModule.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "Blueprint/UserWidget.h"

void AHumanitysFall_DebugGameMode::BeginPlay()
{
	// Apply HUD when player enter the game.
	ApplyHUDChanges();
}

AHumanitysFall_DebugGameMode::AHumanitysFall_DebugGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// Use our HUD class.
	HUDClass = ACharacterHUD::StaticClass();

	// Set default HUD state.
	HUDState = EHUDState::HS_Ingame;
}

void AHumanitysFall_DebugGameMode::ApplyHUDChanges()
{
	
	// Remove previous HUD
	if (currentWidget != nullptr)
	{
		currentWidget->RemoveFromParent();
	}

	switch (HUDState)
	{
		case EHUDState::HS_Ingame:
		{
			ApplyHUD(ingameHUDClass, false, false);
			break;
		}

		case EHUDState::HS_Inventory:
		{
			ApplyHUD(inventoryHUDClass, true, true);
			break;
		}
		
		case EHUDState::HS_Shop_General:
		{
			ApplyHUD(shopGeneralHUDClass, true, true);
			break;
		}

		default:
		{
			ApplyHUD(ingameHUDClass, false, false);
			break;
		}
	}
}

uint8 AHumanitysFall_DebugGameMode::GetHUDState()
{
	return HUDState;
}

void AHumanitysFall_DebugGameMode::ChangeHUDState(uint8 newState)
{
	HUDState = newState;
	ApplyHUDChanges();
}

bool AHumanitysFall_DebugGameMode::ApplyHUD(TSubclassOf<class UUserWidget> widgetToApply, bool bShowMouseCursor, bool EnableClickEvents)
{
	// Get refference to player and controller.
	AHumanitysFall_DebugCharacter* myCharacter = Cast<AHumanitysFall_DebugCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	APlayerController* myController = GetWorld()->GetFirstPlayerController();

	// Null check for widget.
	if (widgetToApply != nullptr)
	{
		// Set mouse visibility and click events according to params.
		myController->bShowMouseCursor = bShowMouseCursor;
		myController->bEnableClickEvents = EnableClickEvents;

		// Create widget.
		currentWidget = CreateWidget<UUserWidget>(GetWorld(), widgetToApply);

		if (currentWidget != nullptr)
		{
			currentWidget->AddToViewport();
			return true;
		}
		else
			return false;
	}
	else
		return false;
}
