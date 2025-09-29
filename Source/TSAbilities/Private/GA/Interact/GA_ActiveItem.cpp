#include "GA/Interact/GA_ActiveItem.h"
#include "GameFramework/Character.h"

UGA_ActiveItem::UGA_ActiveItem()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_ActiveItem::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get()))
	{
		UE_LOG(LogTemp, Log, TEXT("UGA_ActiveItem Ability Activated : Success"));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("UGA_ActiveItem Ability Activated : Fail"));	
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
