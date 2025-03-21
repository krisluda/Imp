// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class Imp : ModuleRules {
	public Imp(ReadOnlyTargetRules Target) : base(Target) {
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"EnhancedInput", 
			"UMG", 
			"OnlineSubsystem", 
			"OnlineSubsystemEOS", 
			"Niagara", 
			"GameplayAbilities", 
			"GameplayTags", 
			"GameplayTasks" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true

		PublicIncludePaths.AddRange(new string[] {
			ModuleDirectory,
			Path.Combine(ModuleDirectory, "Actors"),
			Path.Combine(ModuleDirectory, "Game"),
			Path.Combine(ModuleDirectory, "GAS"),
			Path.Combine(ModuleDirectory, "Global"),
			Path.Combine(ModuleDirectory, "Inventory"),
			Path.Combine(ModuleDirectory, "Player"),
		});
	}
}
