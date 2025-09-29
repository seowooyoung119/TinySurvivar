#include "Player/TSPlayerState.h"
#include "AttributeSet/TSAttributeSet.h"
#include "Component/TSAbilitySystemComponent.h"

ATSPlayerState::ATSPlayerState()
{
	TSAbilitySystemComponent = CreateDefaultSubobject<UTSAbilitySystemComponent>("ASC");
	TSAbilitySystemComponent->SetIsReplicated(true);
	TSAbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	TSAttributeSet = CreateDefaultSubobject<UTSAttributeSet>("AS");
}
