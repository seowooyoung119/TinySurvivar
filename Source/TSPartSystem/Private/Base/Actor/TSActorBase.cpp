#include "Base/Actor/TSActorBase.h"
#include "Component/TSAbilitySystemComponent.h"

ATSActorBase::ATSActorBase()
{
	PrimaryActorTick.bCanEverTick = false;

	// 컴포넌트 생성은 정말 써야 하는 곳에서만 할 것.
}

void ATSActorBase::BeginPlay()
{
	Super::BeginPlay();
	
	// 생성하는 클래스의 어빌리티 초기화는 이렇게 해야 한다.
	// SetInitAbilityActorInfo();
}

void ATSActorBase::SetAbilitySystemComp_And_AttributeSet(UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	// 여기서 생성했을 경우 실행 X
	if (bWantCreateInitAbilityHere) return;

	// 연결
	TSAbilitySystemComponent = ASC;
	TSAttributeSet = AS;
}

void ATSActorBase::SetInitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor)
{
	// 아무것도 설정하지 않은 경우 : 기본으로 this, this
	if (!bWantSetInitAbilityOwnerActor)
	{
		TSAbilitySystemComponent->InitAbilityActorInfo(this, this); 
	}

	// 소유자가 다른 경우 : 소유자는 Owner, 아바타는 this 
	if (InOwnerActor && !InAvatarActor)
	{
		TSAbilitySystemComponent->InitAbilityActorInfo(InOwnerActor, this); 
	}
}

