// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using System.IO;
using System;
using UnrealBuildTool;

/**
 * Builds the actual Unreal Plugin for Realsense 2.0 Library
 * */
public class RealSensePlugin : ModuleRules
{

    private string ModulePath
    {
        get { return ModuleDirectory; }
    }

    private string ThirdPartyPath
    {
        get { return Path.GetFullPath(Path.Combine(ModulePath, "../ThirdParty/")); }
    }

    private string BinariesPath
    {
        get { return Path.GetFullPath(Path.Combine(ModulePath, "../../Binaries/")); }
    }

    public RealSensePlugin(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        bUseRTTI = true;
        bEnableExceptions = true;
		
		PublicIncludePaths.AddRange(
			new string[] {
                "RealSensePlugin/Public",
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
                "RealSensePlugin/Private",
				// ... add other private include paths required here ...
			}
			);


        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "Engine",
                "InputCore",
                "RHI",
                "RenderCore",
                "LRS2",
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"EditorStyle",
                "AssetRegistry",
                "Slate",
                "SlateCore",
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
