// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.IO;

public class MultiplanetTemplate : ModuleRules
{
    private string ModulePath
    {
        get { return ModuleDirectory; }
    }

    private string ThirdPartyPath
    {
        get { return Path.GetFullPath(Path.Combine(ModulePath, "../../ThirdParty/")); }
    }

    public MultiplanetTemplate(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "RuntimeMeshComponent" });
        //PublicIncludePaths.AddRange(new string[] { "RuntimeMeshComponent/Public", "RuntimeMeshComponent/Classes" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true

        // All the default constructor stuff
        LoadLib(Target, "LibNoise", "LIB_NOISE");
    }

    public bool LoadLib(ReadOnlyTargetRules Target, string libPath, string libName)
    {
        bool isLibrarySupported = false;

        if (Target.Platform == UnrealTargetPlatform.Win64 || Target.Platform == UnrealTargetPlatform.Win32)
        {
            isLibrarySupported = true;
            string PlatformString = (Target.Platform == UnrealTargetPlatform.Win64) ? "x64" : "x86";
            string LibrariesPath = Path.Combine(ThirdPartyPath, libPath, "Libraries");

            PublicSystemLibraryPaths.Add(Path.Combine(LibrariesPath, libPath + "." + PlatformString + ".lib"));
            PublicAdditionalLibraries.Add(Path.Combine(LibrariesPath, libPath + "." + PlatformString + ".lib"));
            PublicIncludePaths.Add(Path.Combine(ThirdPartyPath, libPath, "Includes"));
        }
        PublicDefinitions.Add(string.Format("WITH_" + libName + "_BINDING={0}", isLibrarySupported ? 1 : 0));
        return isLibrarySupported;
    }

}
