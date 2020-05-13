// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "SpaceDesignerStyle.h"

class FSpaceDesignerCommands : public TCommands<FSpaceDesignerCommands>
{
public:

	FSpaceDesignerCommands()
		: TCommands<FSpaceDesignerCommands>(TEXT("SpaceDesigner"), NSLOCTEXT("Contexts", "SpaceDesigner", "SpaceDesigner Plugin"), NAME_None, FSpaceDesignerStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;
	
public:

	TSharedPtr<FUICommandInfo> CelestialBodyGenerator;
	TSharedPtr<FUICommandInfo> DisplayDebugOrbitPath;
	TSharedPtr<FUICommandInfo> DisplayDebugPoles;

};