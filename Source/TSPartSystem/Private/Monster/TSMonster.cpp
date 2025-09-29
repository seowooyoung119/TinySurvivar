#include "Monster/TSMonster.h"
#include "AttributeSet/TSAttributeSet.h"
#include "Component/TSAbilitySystemComponent.h"


ATSMonster::ATSMonster()
{
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;
	SetReplicatingMovement(true);
	
	bWantCreateInitAbilityHere = true;
	bWantSetInitAbilityOwnerActor = false;

	TSAbilitySystemComponent = CreateDefaultSubobject<UTSAbilitySystemComponent>("ASC");
	TSAbilitySystemComponent->SetIsReplicated(true);
	TSAbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	TSAttributeSet = CreateDefaultSubobject<UTSAttributeSet>("AS");
}

void ATSMonster::BeginPlay()
{
	Super::BeginPlay();
	SetInitAbilityActorInfo();
}
