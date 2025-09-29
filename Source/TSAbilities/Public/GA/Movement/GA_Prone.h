#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_Prone.generated.h"

/**
 * 
 */
UCLASS()
class TSABILITIES_API UGA_Prone : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_Prone();

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;
	
};
