// AbilityDataRow.h
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameplayTagContainer.h"
#include "AbilityDataRow.generated.h"

class UGameplayAbility;

USTRUCT(BlueprintType)
struct FAbilityDataRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	// 어빌리티 이름 : 블루프린트 상 존재하는 어빌리티 클래스 이름과 같은 이름을 쓰세요. 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Ability")
	FText DisplayName;

	// 어빌리티 설명 : 이 어빌리티가 무슨 능력을 부여하는지 적으세요.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Ability")
	FText Description;

	// GAS 어빌리티 클래스 : 어빌티리를 담는 공간입니다.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Ability")
	TSubclassOf<UGameplayAbility> AbilityClass;

	// 어빌리티 태그 표시용 : 이 어빌리티가 어떤 태그를 가지고 있는지 표시합니다.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Ability")
	FGameplayTagContainer AbilityTag;
};
