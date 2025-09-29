#include "GA/Movement/GA_Sprint.h"
#include "GameFramework/Character.h"

UGA_Sprint::UGA_Sprint()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_Sprint::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get()))
	{
		UE_LOG(LogTemp, Log, TEXT("UGA_Sprint Ability Activated : Success"));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("UGA_Sprint Ability Activated : Fail"));	
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
