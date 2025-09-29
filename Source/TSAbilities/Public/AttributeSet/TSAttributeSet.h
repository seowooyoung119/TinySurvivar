#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "TSAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class TSABILITIES_API UTSAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UTSAttributeSet();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

protected:

	void ClampInBeforeChange(const FGameplayAttribute& Attribute, float& NewValue) const;
	void ClampInPostGameplayEffect(const struct FGameplayEffectModCallbackData& Data);
	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

protected:

	//---------------------------------------------
	// 원시 능력치 : 스테이터스
	//---------------------------------------------

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing=OnRep_Attack, Category="Primary Attributes")
	FGameplayAttributeData Attack;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing=OnRep_MaxAttack, Category="Primary Attributes")
	FGameplayAttributeData MaxAttack;

	
	//---------------------------------------------
	// 필수 능력치 : 절대 없어선 안 될 기반 능력치
	//---------------------------------------------
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing=OnRep_Health, Category="vital Attributes")
	FGameplayAttributeData Health;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing=OnRep_MaxHealth, Category="vital Attributes")
	FGameplayAttributeData MaxHealth;
	
	//--------------------------------------------
	// 생존 능력치 : 생존 관련 능력치
	//--------------------------------------------
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing=OnRep_Hunger, Category="Survival Attributes")
	FGameplayAttributeData Hunger;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing=OnRep_MaxHunger, Category="Survival Attributes")
	FGameplayAttributeData MaxHunger;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing=OnRep_Thirst, Category="Survival Attributes")
	FGameplayAttributeData Thirst;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing=OnRep_MaxThirst, Category="Survival Attributes")
	FGameplayAttributeData MaxThirst;
	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//---------------------------------------------
	// 원시 능력치 : 스테이터스
	//---------------------------------------------
	
	ATTRIBUTE_ACCESSORS(UTSAttributeSet, Attack);
	ATTRIBUTE_ACCESSORS(UTSAttributeSet, MaxAttack);

	//---------------------------------------------
	// 필수 능력치 : 절대 없어선 안 될 기반 능력치
	//---------------------------------------------
	
	ATTRIBUTE_ACCESSORS(UTSAttributeSet, Health);
	ATTRIBUTE_ACCESSORS(UTSAttributeSet, MaxHealth);

	//--------------------------------------------
	// 생존 능력치 : 생존 관련 능력치
	//--------------------------------------------

	ATTRIBUTE_ACCESSORS(UTSAttributeSet, Hunger);
	ATTRIBUTE_ACCESSORS(UTSAttributeSet, MaxHunger);

	ATTRIBUTE_ACCESSORS(UTSAttributeSet, Thirst);
	ATTRIBUTE_ACCESSORS(UTSAttributeSet, MaxThirst);
	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//---------------------------------------------
	// 원시 능력치 : 스테이터스
	//---------------------------------------------

	UFUNCTION()
	void OnRep_Attack(const FGameplayAttributeData& OldAttack) const;
	UFUNCTION()
	void OnRep_MaxAttack(const FGameplayAttributeData& OldMaxAttack) const;

	//---------------------------------------------
	// 필수 능력치 : 절대 없어선 안 될 기반 능력치
	//---------------------------------------------
	
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;
	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;

	//--------------------------------------------
	// 생존 능력치 : 생존 관련 능력치
	//--------------------------------------------
	
	UFUNCTION()
	void OnRep_Hunger(const FGameplayAttributeData& OldHunger) const;
	UFUNCTION()
	void OnRep_MaxHunger(const FGameplayAttributeData& OldMaxHunger) const;

	UFUNCTION()
	void OnRep_Thirst(const FGameplayAttributeData& OldThirst) const;
	UFUNCTION()
	void OnRep_MaxThirst(const FGameplayAttributeData& OldMaxThirst) const;
};
