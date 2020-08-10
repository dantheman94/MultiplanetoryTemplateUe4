// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "SpaceDesigner.h"
#include "SpaceDesignerStyle.h"
#include "SpaceDesignerCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"

static const FName SpaceDesignerTabName("SpaceDesigner");

#define LOCTEXT_NAMESPACE "FSpaceDesignerModule"

void FSpaceDesignerModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FSpaceDesignerStyle::Initialize();
	FSpaceDesignerStyle::ReloadTextures();

	FSpaceDesignerCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FSpaceDesignerCommands::Get().CelestialBodyGenerator,
		FExecuteAction::CreateRaw(this, &FSpaceDesignerModule::PluginButtonClicked),
		FCanExecuteAction());
		
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	
	{
		TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());
		MenuExtender->AddMenuExtension("WindowLayout", EExtensionHook::After, PluginCommands, FMenuExtensionDelegate::CreateRaw(this, &FSpaceDesignerModule::AddMenuExtension));

		LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
	}
	
	{
		TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
		ToolbarExtender->AddToolBarExtension("Settings", EExtensionHook::After, PluginCommands, FToolBarExtensionDelegate::CreateRaw(this, &FSpaceDesignerModule::AddToolbarExtension));
		
		LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);
	}
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(SpaceDesignerTabName, FOnSpawnTab::CreateRaw(this, &FSpaceDesignerModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FSpaceDesignerTabTitle", "SpaceDesigner"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FSpaceDesignerModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FSpaceDesignerStyle::Shutdown();

	FSpaceDesignerCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(SpaceDesignerTabName);
}

TSharedRef<SDockTab> FSpaceDesignerModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	FText WidgetText = FText::Format(
		LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
		FText::FromString(TEXT("FSpaceDesignerModule::OnSpawnPluginTab")),
		FText::FromString(TEXT("SpaceDesigner.cpp"))
		);

	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			// Put your tab content here!
			SNew(SBox)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(WidgetText)
			]
		];
}

void FSpaceDesignerModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->InvokeTab(SpaceDesignerTabName);
}

TSharedRef<SWidget> FSpaceDesignerModule::FillComboBox(TSharedPtr<class FUICommandList> Commands)
{
	FMenuBuilder MenuBuilder(true, Commands);

	MenuBuilder.BeginSection("CelestialGenerator", TAttribute<FText>(FText::FromString("Celestial Bodies")));
	{
		MenuBuilder.AddMenuEntry(FSpaceDesignerCommands::Get().CelestialBodyGenerator);
		///MenuBuilder.AddMenuEntry(FSpaceDesignerCommands::Get().DisplayDebugOrbitPath);
		///MenuBuilder.AddMenuEntry(FSpaceDesignerCommands::Get().DisplayDebugPoles);
	}
	MenuBuilder.EndSection();

	return MenuBuilder.MakeWidget();
}

void FSpaceDesignerModule::AddMenuExtension(FMenuBuilder& Builder)
{
	///Builder.AddMenuEntry(FSpaceDesignerCommands::Get().OpenPluginWindow);
}

void FSpaceDesignerModule::AddToolbarExtension(FToolBarBuilder& Builder)
{
	FUIAction toolbarAction;
	
	///Builder.AddToolBarButton(FSpaceDesignerCommands::Get().opee);
	Builder.AddSeparator();
	Builder.AddComboButton(toolbarAction, FOnGetContent::CreateRaw(this, &FSpaceDesignerModule::FillComboBox, PluginCommands));
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSpaceDesignerModule, SpaceDesigner)