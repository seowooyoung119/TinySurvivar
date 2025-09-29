#pragma once

#include "CoreMinimal.h"
#include "Base/Character/TSCharacterBase.h"
#include "TSPlayer.generated.h"

class UPlayerInputDataAsset;
struct FInputActionValue;

UCLASS()
class TSPARTSYSTEM_API ATSPlayer : public ATSCharacterBase
{
	GENERATED_BODY()

public:
	ATSPlayer();

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	virtual void initializeAbilities();
	
	virtual void OnMove(const FInputActionValue& Value);
	virtual void OnLook(const FInputActionValue& Value);
	virtual void OnJump(const FInputActionValue& Value);
	
	virtual void OnSprint(const FInputActionValue& Value);
	virtual void OnProne(const FInputActionValue& Value);
	virtual void OnCrouch(const FInputActionValue& Value);

	virtual void OnActiveItem(const FInputActionValue& Value);
	virtual void OnPickUpItem(const FInputActionValue& Value);
	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	UPROPERTY(EditAnywhere)
	TObjectPtr<UPlayerInputDataAsset> PlayerInputDataAsset;
};
