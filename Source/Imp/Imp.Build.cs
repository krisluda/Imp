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
			"GameplayTasks", 
			"NetCore"
			});

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true

		PublicIncludePaths.AddRange(new string[] {
			ModuleDirectory,
			Path.Combine(ModuleDirectory, "Actors"),
			Path.Combine(ModuleDirectory, "Character"),
			Path.Combine(ModuleDirectory, "Data"),
			Path.Combine(ModuleDirectory, "Game"),
			Path.Combine(ModuleDirectory, "Game/PlayerController"),
			Path.Combine(ModuleDirectory, "Game/PlayerState"),
			Path.Combine(ModuleDirectory, "GAS"),
			Path.Combine(ModuleDirectory, "GAS/Abilities"),
			Path.Combine(ModuleDirectory, "GAS/ExecCalc"),
			Path.Combine(ModuleDirectory, "Global"),
			Path.Combine(ModuleDirectory, "Libraries"),
			Path.Combine(ModuleDirectory, "Input"),
			Path.Combine(ModuleDirectory, "Interfaces"),
			Path.Combine(ModuleDirectory, "Inventory"),
			Path.Combine(ModuleDirectory, "Projectiles"),
			Path.Combine(ModuleDirectory, "UI"),
			Path.Combine(ModuleDirectory, "UI/WidgetControllers"),
		});
	}
}
