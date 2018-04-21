using System;
using System.IO;
using UnrealBuildTool;

public class RealSensePlugin : ModuleRules 
{
    private string ModulePath
    {
        get { return ModuleDirectory; }
    }

    private string ThirdPartyPath
    {
        get { return Path.GetFullPath(Path.Combine(ModulePath, "../../ThirdParty/")); }
    }

    private string BinariesPath
    {
        get { return Path.GetFullPath(Path.Combine(ModulePath, "../../Binaries/")); }
    }

    private string LibraryPath
    {
        get { return Path.GetFullPath(Path.Combine(ThirdPartyPath, "RSSDK", "lib")); }
    }

    public RealSensePlugin(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(
			new string[] {
                "RealSensePlugin/Public",
                 Path.Combine(ThirdPartyPath, "RSSDK", "include"),
            }
		);

        PrivateIncludePaths.AddRange(
            new string[] {
                "RealSensePlugin/Private",
            }
       );


        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
			}
            );


        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
			}
        );

        if ((Target.Platform == UnrealTargetPlatform.Win64) || (Target.Platform == UnrealTargetPlatform.Win32))
        {
            string PlatformString = (Target.Platform == UnrealTargetPlatform.Win64) ? "x64" : "Win32";

            PublicAdditionalLibraries.Add(Path.Combine(LibraryPath, PlatformString, "libpxc.lib"));
        }


        //string RealSenseDirectory = Environment.GetEnvironmentVariable("RSSDK_DIR");
        //string RealSenseIncludeDirectory = RealSenseDirectory + "include";
        //string RealSenseLibrary32Directory = RealSenseDirectory + "lib\\Win32\\libpxc.lib";
        //string RealSenseLibrary64Directory = RealSenseDirectory + "lib\\x64\\libpxc.lib";

        //PublicIncludePaths.Add(RealSenseIncludeDirectory);
        //PublicAdditionalLibraries.Add(RealSenseLibrary32Directory);
        //PublicAdditionalLibraries.Add(RealSenseLibrary64Directory);
    }
}