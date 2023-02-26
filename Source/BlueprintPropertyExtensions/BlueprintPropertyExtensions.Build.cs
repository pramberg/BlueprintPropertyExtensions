// Copyright Viktor Pramberg. All Rights Reserved.

using UnrealBuildTool;

public class BlueprintPropertyExtensions : ModuleRules
{
	public BlueprintPropertyExtensions(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicDependencyModuleNames.AddRange
		(
			new string[]
			{
				"Core",
			}
		);
		
		PrivateDependencyModuleNames.AddRange
		(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"GameplayTags",
				"GameplayTagsEditor",
				"DeveloperSettings",
				"UnrealEd",
				"PropertyEditor",
				"ClassViewer",
				"PropertyPath",
				"GraphEditor",
				"BlueprintGraph",
			}
		);
	}
}
