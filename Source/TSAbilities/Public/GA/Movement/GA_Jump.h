#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_Jump.generated.h"

class UAnimMontage;

/**
 * 
 */
UCLASS()
class TSABILITIES_API UGA_Jump : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_Jump();


	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

	virtual void CancelAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		bool bReplicateCancelAbility) override;
	
	virtual void EndAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		bool bReplicateEndAbility,
		bool bWasCancelled) override;

		
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	

	UFUNCTION()
	void OnJumpCompleteCallBack();
	
	UFUNCTION()
	void OnJumpInterruptedCallBack();
	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	

	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> JumpAnimMontage;

	UPROPERTY()
	TObjectPtr<ACharacter> OwnerCharacter;
	
};