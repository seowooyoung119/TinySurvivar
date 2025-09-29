#pragma once

#include "CoreMinimal.h"
#include "Base/Character/TSCharacterBase.h"
#include "TSMonster.generated.h"

UCLASS()
class TSPARTSYSTEM_API ATSMonster : public ATSCharacterBase
{
	GENERATED_BODY()

public:
	ATSMonster();

protected:
	virtual void BeginPlay() override;
};
