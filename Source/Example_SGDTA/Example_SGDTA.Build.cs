// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Example_SGDTA : ModuleRules
{
	public Example_SGDTA(ReadOnlyTargetRules Target) : base(Target)
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
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"SGDynamicTextAssetsRuntime",
		});

		PublicIncludePaths.AddRange(new string[] {
			"Example_SGDTA",
			"Example_SGDTA/Variant_Shooter",
			"Example_SGDTA/Variant_Shooter/AI",
			"Example_SGDTA/Variant_Shooter/UI",
			"Example_SGDTA/Variant_Shooter/Weapons"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
