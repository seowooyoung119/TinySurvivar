#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "TSPlayerState.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class TSPARTSYSTEM_API ATSPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ATSPlayerState();


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	

	// 어빌리티 시스템 컴포넌트
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities | System")
	TObjectPtr<UAbilitySystemComponent> TSAbilitySystemComponent;

	// 어트리뷰트 세트
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities | System")
	TObjectPtr<UAttributeSet> TSAttributeSet;
};
