// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GGJProject : ModuleRules
{
	public GGJProject(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "MediaAssets", "Niagara", "FMODStudio" });
		
		PrivateDependencyModuleNames.AddRange(new string[] {  "Slate", "SlateCore" });

	}
}
