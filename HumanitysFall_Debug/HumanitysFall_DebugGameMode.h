// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/GameMode.h"
#include "HumanitysFall_DebugGameMode.generated.h"

UCLASS(minimalapi)
class AHumanitysFall_DebugGameMode : public AGameMode
{
	GENERATED_BODY()

	virtual void BeginPlay() override;

public:
	AHumanitysFall_DebugGameMode();

	enum EHUDState : uint8
	{
		HS_Ingame,
		HS_Inventory,
		HS_Shop_General,
		HS_Shop_Weapon
	};

	// Check the hud state, and applies different huds accondingly.
	void ApplyHUDChanges();

	// Getter for HUDState.
	uint8 GetHUDState();

	// Setter for HUDState.
	UFUNCTION(BlueprintCallable, Category = "HUD Functions")
	void ChangeHUDState(uint8 newState);

	// Applies HUD to screen.
	bool ApplyHUD(TSubclassOf<class UUserWidget> widgetToApply, bool bShowMouseCursor, bool EnableClickEvents);

protected:

	uint8 HUDState;

	// HUD state to be shown ingame
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUDWidgets", Meta = (BlueprintProtected = "true"))
	TSubclassOf<class UUserWidget> ingameHUDClass;

	// HUD state to be shown in the inventory
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUDWidgets", Meta = (BlueprintProtected = "true"))
	TSubclassOf<class UUserWidget> inventoryHUDClass;

	// HUD state to be shown in the shop
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUDWidgets", Meta = (BlueprintProtected = "true"))
	TSubclassOf<class UUserWidget> shopGeneralHUDClass;

	UPROPERTY()
	class UUserWidget* currentWidget;
};



