#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PlayerInputDataAsset.generated.h"

class UInputAction;
class UInputMappingContext;

/**
 * 
 */
UCLASS()
class TSPARTSYSTEM_API UPlayerInputDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, Category = "PlayerMovment | MappingContext")
	UInputMappingContext* PlayerInputMappingContext;

	UPROPERTY(EditAnywhere, Category = "PlayerMovment | Movement")
	UInputAction* MoveAction;
	
	UPROPERTY(EditAnywhere, Category = "PlayerMovment | Movement")
	UInputAction* LookAction;
	
	UPROPERTY(EditAnywhere, Category = "PlayerMovment | Movement")
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, Category = "PlayerMovment | Movement")
	UInputAction* SprintAction;

	UPROPERTY(EditAnywhere, Category = "PlayerMovment | Movement")
	UInputAction* ProneAction;
	
	UPROPERTY(EditAnywhere, Category = "PlayerMovment | Movement")
	UInputAction* CrouchAction;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
	
	UPROPERTY(EditAnywhere, Category = "PlayerMovment | Interact | Item")
	UInputAction* ActiveItemAction;
	
	UPROPERTY(EditAnywhere, Category = "PlayerMovment | Interact | Item")
	UInputAction* PickUpItemAction;

	UPROPERTY(EditAnywhere, Category = "PlayerMovment | Interact | Item")
	UInputAction* DropItemAction;
};
