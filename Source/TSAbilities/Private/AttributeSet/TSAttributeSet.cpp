#include "AttributeSet/TSAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"

UTSAttributeSet::UTSAttributeSet()
{
	InitAttack(10.f);
	InitMaxAttack(9999.f);
	
	///////////////////////////////

	InitHealth(100.f);
	InitMaxHealth(100.f);

	///////////////////////////////

	InitHunger(100.f);
	InitMaxHunger(100.f);

	InitThirst(100.f);
	InitMaxThirst(100.f);
}

void UTSAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UTSAttributeSet, Attack, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTSAttributeSet, MaxAttack, COND_None, REPNOTIFY_Always);

	//////////////////////////////////////////////////////////////////////////////////////

	DOREPLIFETIME_CONDITION_NOTIFY(UTSAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTSAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);

	//////////////////////////////////////////////////////////////////////////////////////

	DOREPLIFETIME_CONDITION_NOTIFY(UTSAttributeSet, Hunger, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTSAttributeSet, MaxHunger, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UTSAttributeSet, Thirst, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTSAttributeSet, MaxThirst, COND_None, REPNOTIFY_Always);
}

void UTSAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	ClampInBeforeChange(Attribute, NewValue);
}

void UTSAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	ClampInBeforeChange(Attribute, NewValue);
}

void UTSAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	ClampInPostGameplayEffect(Data);
}

void UTSAttributeSet::ClampInBeforeChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	if (Attribute == GetAttackAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxAttack());
	}
	else if (Attribute == GetMaxAttackAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, 9999.f);
	}

	////////////////////////////////////////////////////////////////

	else if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetHealth());
	}
	else if (Attribute == GetMaxHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, 9999.f);
	}
	//////////////////////////////////////////////////////////////////

	else if (Attribute == GetHungerAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHunger());
	}
	else if (Attribute == GetMaxHungerAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, 9999.f);
	}
	else if (Attribute == GetThirstAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxThirst());	
	}
	else if (Attribute == GetMaxThirstAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, 9999.f);
	}
}

void UTSAttributeSet::ClampInPostGameplayEffect(const struct FGameplayEffectModCallbackData& Data)
{
	if (Data.EvaluatedData.Attribute == GetAttackAttribute())
	{
		SetAttack(FMath::Clamp(GetAttack(), 0.f, GetMaxAttack()));
	}
	else if (Data.EvaluatedData.Attribute == GetMaxAttackAttribute())
	{
		SetMaxAttack(FMath::Clamp(GetMaxAttack(), 0.f, 9999.f));
	}

	////////////////////////////////////////////////////////////////

	else if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
	}
	else if (Data.EvaluatedData.Attribute == GetMaxHealthAttribute())
	{
		SetMaxHealth(FMath::Clamp(GetMaxHealth(), 0.f, 9999.f));
	}
	//////////////////////////////////////////////////////////////////

	else if (Data.EvaluatedData.Attribute == GetHungerAttribute())
	{
		SetHunger(FMath::Clamp(GetHunger(), 0.f, GetMaxHunger()));
	}
	else if (Data.EvaluatedData.Attribute == GetMaxHungerAttribute())
	{
		SetMaxHunger(FMath::Clamp(GetMaxHunger(), 0.f, 9999.f));
	}
	else if (Data.EvaluatedData.Attribute == GetThirstAttribute())
	{
		SetThirst(FMath::Clamp(GetThirst(), 0.f, GetMaxThirst()));
	}
	else if (Data.EvaluatedData.Attribute == GetMaxThirstAttribute())
	{
		SetMaxThirst(FMath::Clamp(GetMaxThirst(), 0.f, 9999.f));
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UTSAttributeSet::OnRep_Attack(const FGameplayAttributeData& OldAttack) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTSAttributeSet, Attack, OldAttack);
}

void UTSAttributeSet::OnRep_MaxAttack(const FGameplayAttributeData& OldMaxAttack) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTSAttributeSet, MaxAttack, OldMaxAttack);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UTSAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTSAttributeSet, Health, OldHealth);
}
void UTSAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTSAttributeSet, MaxHealth, OldMaxHealth);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UTSAttributeSet::OnRep_Hunger(const FGameplayAttributeData& OldHunger) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTSAttributeSet, Hunger, OldHunger);
}

void UTSAttributeSet::OnRep_MaxHunger(const FGameplayAttributeData& OldMaxHunger) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTSAttributeSet, MaxHunger, OldMaxHunger);
}

void UTSAttributeSet::OnRep_Thirst(const FGameplayAttributeData& OldThirst) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTSAttributeSet, Thirst, OldThirst);
}

void UTSAttributeSet::OnRep_MaxThirst(const FGameplayAttributeData& OldMaxThirst) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTSAttributeSet, MaxThirst, OldMaxThirst);
}

