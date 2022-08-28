// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class vr_Simulation : ModuleRules
{
	public vr_Simulation(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
