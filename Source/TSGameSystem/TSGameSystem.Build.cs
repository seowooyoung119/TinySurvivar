using UnrealBuildTool;

public class TSGameSystem : ModuleRules
{
    public TSGameSystem(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                
                // GAS
                "GameplayAbilities", "GameplayTasks", "GameplayTags",
                
                //
                "TSAbilities", "TSPartSystem",
            }
        );
        
        PublicIncludePaths.AddRange(new string[] { "TSGameSystem/Public" });
        PrivateIncludePaths.AddRange(new string[] { "TSGameSystem/Private" });
    }
}