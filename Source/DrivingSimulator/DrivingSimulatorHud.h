// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/HUD.h"
#include "DrivingSimulatorHud.generated.h"


UCLASS(config = Game)
class ADrivingSimulatorHud : public AHUD
{
	GENERATED_BODY()

public:
	ADrivingSimulatorHud();

	/** Font used to render the vehicle info */
	UPROPERTY()
	UFont* HUDFont;

	// Begin AHUD interface
	virtual void DrawHUD() override;
	// End AHUD interface
};
