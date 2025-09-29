#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_PickUpItem.generated.h"

/**
 * 
 */
UCLASS()
class TSABILITIES_API UGA_PickUpItem : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_PickUpItem();

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;
};
