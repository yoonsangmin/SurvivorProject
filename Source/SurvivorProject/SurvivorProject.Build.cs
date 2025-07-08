// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SurvivorProject : ModuleRules
{
	public SurvivorProject(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(new string[] { "SurvivorProject" });
		
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "NavigationSystem", "AIModule", "Niagara", "EnhancedInput", "GameplayAbilities", "GameplayTasks", "GameplayTags" });
    }
}
