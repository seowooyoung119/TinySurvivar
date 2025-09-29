#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TSItemDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class TSPARTSYSTEM_API UTSItemDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	// 여러 아이템 테이블 관리
	UPROPERTY(EditAnywhere, Category="Ability")
	TMap<FName, UDataTable*> ItemTables;	
};
