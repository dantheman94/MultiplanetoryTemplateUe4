// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "SpaceDesignerCommands.h"

#define LOCTEXT_NAMESPACE "FSpaceDesignerModule"

void FSpaceDesignerCommands::RegisterCommands()
{
	UI_COMMAND(CelestialBodyGenerator, "Celestial Body Generator", "Create a new Celestial Body for this window", EUserInterfaceActionType::Button, FInputGesture());
	//UI_COMMAND(DisplayDebugOrbitPath, "Show/Hide Celestial Body orbiting paths", EUserInterfaceActionType::Button, FInputGesture());
	//UI_COMMAND(DisplayPoles, "Show/Hide Celestial Body north & south poles", EUserInterfaceActionType::ToggleButton, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
