using UnrealBuildTool;

public class TSPartSystem : ModuleRules
{
    public TSPartSystem(ReadOnlyTargetRules Target) : base(Target)
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
                "CoreUObject", "Engine",
                
                // 입력
                "InputCore", "EnhancedInput",
                
                // GAS
                "GameplayAbilities", "GameplayTasks", "GameplayTags",
                
                //
                "TSAbilities", "TSGameSystem",
            }
        );
        
        PublicIncludePaths.AddRange(new string[] { "TSPartSystem/Public" });
        PrivateIncludePaths.AddRange(new string[] { "TSPartSystem/Private" });
    }
}