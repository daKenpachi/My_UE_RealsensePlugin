// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using System.IO;
using System;
using UnrealBuildTool;

public class Librealsense2 : ModuleRules
{

    private string ModulePath
    {
        get { return ModuleDirectory;  }
    }

    private string BinariesPath
    {
        get { return Path.GetFullPath(Path.Combine(ModulePath, "../Binaries"));  }
    }

    public Librealsense2(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        Type = ModuleType.External;

        LoadLibrealsense2(Target);
    }

    public bool LoadLibrealsense2(ReadOnlyTargetRules Target)
    {

        bool isLibrarySupported = false;
        bool bDebug = (Target.Configuration == UnrealTargetConfiguration.Debug); // doesnt work:  && BuildConfiguration.bDebugBuildsActuallyUseDebugCRT);
		
        if(Target.Platform == UnrealTargetPlatform.Win64)
        {
            isLibrarySupported = true;

            PublicAdditionalLibraries.Add(Path.Combine(ModulePath, "librealsense2/bin/x64/realsense2.lib"));
        }
        else if (Target.Platform == UnrealTargetPlatform.Win32)
        {
            isLibrarySupported = true;

            PublicAdditionalLibraries.Add(Path.Combine(ModulePath, "librealsense2/bin/x86/realsense2.lib"));
        }

        if (isLibrarySupported)
        {
            PublicIncludePaths.Add(Path.Combine(ModulePath, "librealsense2/include/librealsense2"));

            bEnableExceptions = true;
        }

        Definitions.Add(string.Format("WITH_REALSENSE_BINDING={0}", isLibrarySupported ? 1 : 0));

        return isLibrarySupported;
	}
}
