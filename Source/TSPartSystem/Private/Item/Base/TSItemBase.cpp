#include "Item/Base/TSItemBase.h"
#include "AttributeSet/TSAttributeSet.h"
#include "Component/TSAbilitySystemComponent.h"

ATSItemBase::ATSItemBase()
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

void ATSItemBase::BeginPlay()
{
	Super::BeginPlay();
	SetInitAbilityActorInfo();
}

