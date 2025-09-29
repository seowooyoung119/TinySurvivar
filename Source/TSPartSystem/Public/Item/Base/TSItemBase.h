#pragma once

#include "CoreMinimal.h"
#include "Base/Actor/TSActorBase.h"
#include "TSItemBase.generated.h"

UCLASS()
class TSPARTSYSTEM_API ATSItemBase : public ATSActorBase
{
	GENERATED_BODY()

public:
	ATSItemBase();
protected:
	virtual void BeginPlay() override;
};
