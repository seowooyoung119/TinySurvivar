#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "TSGameInstance.generated.h"

class UTSItemDataAsset;
class UTSAbilityDataAsset;

/**
 * 
 */
UCLASS()
class TSGAMESYSTEM_API UTSGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Ability")
	UTSAbilityDataAsset* AbilityAsset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item")
	UTSItemDataAsset* ItemAsset;
};
