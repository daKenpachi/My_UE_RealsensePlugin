using UnrealBuildTool;
using System.IO;

public class MyRealSense : ModuleRules
{
    public MyRealSense(ReadOnlyTargetRules Target) : base(Target)
    {
        PrivateIncludePaths.AddRange(new string[] { "Private" });
        PrivateIncludePaths.AddRange(new string[] { "Public" });

        PublicDependencyModuleNames.AddRange(new string[] { "Engine", "Core" });
    }
}