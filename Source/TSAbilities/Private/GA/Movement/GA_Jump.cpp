#include "GA/Movement/GA_Jump.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitInputPress.h"
#include "GameFramework/Character.h"

UGA_Jump::UGA_Jump()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_Jump::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	UAbilityTask_PlayMontageAndWait* JumpTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, JumpAnimMontage);
	JumpTask->OnCancelled.AddDynamic(this, &ThisClass::OnJumpInterruptedCallBack);
	JumpTask->OnInterrupted.AddDynamic(this, &ThisClass::OnJumpInterruptedCallBack);
	JumpTask->OnCompleted.AddDynamic(this,&ThisClass::OnJumpCompleteCallBack);
	JumpTask->OnBlendOut.AddDynamic(this, &ThisClass::OnJumpCompleteCallBack); 
	JumpTask->ReadyForActivation();
	
	OwnerCharacter = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	if (OwnerCharacter) OwnerCharacter->Jump();
}

void UGA_Jump::CancelAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UGA_Jump::EndAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility,
	bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UGA_Jump::OnJumpCompleteCallBack()
{
	if (OwnerCharacter) OwnerCharacter->StopJumping();
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGA_Jump::OnJumpInterruptedCallBack()
{
	if (OwnerCharacter) OwnerCharacter->StopJumping();
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}