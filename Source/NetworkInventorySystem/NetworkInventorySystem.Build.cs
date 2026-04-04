// Copyright © 2026 bo6eR. All rights reserved.

using UnrealBuildTool;
using System.IO;

public class NetworkInventorySystem : ModuleRules
{
	public NetworkInventorySystem(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange( 
			new []
			{
				Path.Combine(PluginDirectory, "Source/NetworkInventorySystem"),
				Path.Combine(PluginDirectory, "Source/NetworkInventorySystem/Public"),
				Path.Combine(PluginDirectory, "Source/NetworkInventorySystem/Public/Runtime"),
			}
		);
		
		PublicDependencyModuleNames.AddRange(
			new [] 
			{
				"Core", "NetCore", "DeveloperSettings", "GameplayTags"
			}
		);
		
		PrivateDependencyModuleNames.AddRange(
			new []
			{
				"CoreUObject", "Engine"
			}
		);
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[] { }
		);
	}
}
