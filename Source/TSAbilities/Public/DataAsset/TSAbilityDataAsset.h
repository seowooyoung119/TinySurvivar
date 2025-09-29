#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TSAbilityDataAsset.generated.h"

class UDataTable;
/**
 * 
 */
UCLASS()
class TSABILITIES_API UTSAbilityDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	// 여러 AbilityDataTable 관리
	UPROPERTY(EditAnywhere, Category="Ability")
	TMap<FName, UDataTable*> AbilityTables;
	
};
