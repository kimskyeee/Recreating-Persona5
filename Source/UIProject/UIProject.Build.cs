// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UIProject : ModuleRules
{
	public UIProject(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"CommonUI"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"UIProject",
			"UIProject/Variant_Platforming",
			"UIProject/Variant_Combat",
			"UIProject/Variant_Combat/AI",
			"UIProject/Variant_SideScrolling",
			"UIProject/Variant_SideScrolling/Gameplay",
			"UIProject/Variant_SideScrolling/AI"
		});

		// Uncomment if you are using Slate UI
		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
