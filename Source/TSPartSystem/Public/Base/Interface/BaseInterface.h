#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BaseInterface.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;

UINTERFACE()
class UBaseInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TSPARTSYSTEM_API IBaseInterface
{
	GENERATED_BODY()

public:
	virtual void SetAbilitySystemComp_And_AttributeSet(UAbilitySystemComponent* ASC, UAttributeSet* AS) = 0;;
	virtual void SetInitAbilityActorInfo(AActor* InOwnerActor = nullptr, AActor* InAvatarActor = nullptr) = 0;
};
