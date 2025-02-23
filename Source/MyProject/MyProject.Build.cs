// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MyProject : ModuleRules
{
	public MyProject(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "Paper2D" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		PublicIncludePaths.AddRange(new string[] {
			"MyProject/Common/Public",
			"MyProject/Common/Gamemode/Public",
			"MyProject/Utility/Public",
			"MyProject/Player/Public",
			"MyProject/Player/Controller/Public",
			"MyProject/Player/WaterBalloon/Public",
			"MyProject/Player/Item/Public",
			"MyProject/Player/Item/Ride/Public",
			"MyProject/Player/Item/SpawnableItem/Public",
			"MyProject/Player/Animation/Public",
			"MyProject/Widget/Lobby/Public",
			"MyProject/Widget/WaitRoom/Public",
			"MyProject/Widget/Public",
			"MyProject/Computer/Public",
			"MyProject/Map/Public"

		});
		// Uncomment if you are using Slate UI
		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
