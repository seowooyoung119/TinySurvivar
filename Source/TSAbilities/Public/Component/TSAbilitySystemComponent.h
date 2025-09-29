#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "TSAbilitySystemComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TSABILITIES_API UTSAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UTSAbilitySystemComponent();
};
