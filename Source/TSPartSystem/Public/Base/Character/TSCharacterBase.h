#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Base/Interface/BaseInterface.h"
#include "TSCharacterBase.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;

UCLASS()
class TSPARTSYSTEM_API ATSCharacterBase : public ACharacter, public IAbilitySystemInterface, public IBaseInterface
{
	GENERATED_BODY()

public:
	ATSCharacterBase();
protected:
	virtual void BeginPlay() override;
public:
	// IAbilitySystemInterface ~
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return TSAbilitySystemComponent; };
	// ~ IAbilitySystemInterface

	// IBaseInterface ~
	virtual void SetAbilitySystemComp_And_AttributeSet(UAbilitySystemComponent* ASC, UAttributeSet* AS) override;
	virtual void SetInitAbilityActorInfo(AActor* InOwnerActor = nullptr, AActor* InAvatarActor = nullptr) override;
	// ~ IBaseInterface
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

protected:
	// 어빌리티 시스템 컴포넌트, 어트리뷰트 생성 여부
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities | Setting")
	bool bWantCreateInitAbilityHere = true;
	
	// 오너, 아바타 초기화 설정 여부
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities | Setting")
	bool bWantSetInitAbilityOwnerActor = false;
	
	// 어빌리티 시스템 컴포넌트
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities | System")
	TObjectPtr<UAbilitySystemComponent> TSAbilitySystemComponent;

	// 어트리뷰트 세트
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities | System")
	TObjectPtr<UAttributeSet> TSAttributeSet;

};
