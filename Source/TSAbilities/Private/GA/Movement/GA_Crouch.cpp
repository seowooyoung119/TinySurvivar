#include "GA/Movement/GA_Crouch.h"
#include "GameFramework/Character.h"

UGA_Crouch::UGA_Crouch()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_Crouch::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get()))
	{
		UE_LOG(LogTemp, Log, TEXT("UGA_Crouch Ability Activated : Success"));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("UGA_Crouch Ability Activated : Fail"));	
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
