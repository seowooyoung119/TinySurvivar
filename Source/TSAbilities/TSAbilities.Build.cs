using UnrealBuildTool;

public class TSAbilities : ModuleRules
{
    public TSAbilities(ReadOnlyTargetRules Target) : base(Target)
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
                "TSPartSystem"
            }
        );
        
        PublicIncludePaths.AddRange(new string[] { "TSAbilities/Public" });
        PrivateIncludePaths.AddRange(new string[] { "TSAbilities/Private" });
    }
}