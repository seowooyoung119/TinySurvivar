#include "AbilityManager/TSAbilityManagerSubSystem.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "Abilities/GameplayAbility.h"
#include "DataAsset/AbilityDataRow.h"
#include "DataAsset/TSAbilityDataAsset.h"
#include "GameInstance/TSGameInstance.h"

void UTSAbilityManagerSubSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// 어빌리티 매니저 초기화
	if (UTSGameInstance* GI = Cast<UTSGameInstance>(GetGameInstance()))
	{
		if (GI->AbilityAsset)
		{
			AbilityTables = GI->AbilityAsset->AbilityTables;
			for (auto Table : AbilityTables)
			{
				UE_LOG(LogTemp, Log, TEXT("Subsystem loaded Ability DataTable: %s"), *Table.Key.ToString());
			}
		}
	}
}

void UTSAbilityManagerSubSystem::GiveAbilityByTag(UAbilitySystemComponent* ASC, FGameplayTag DesiredTag, FName TableKey, const int32 AbilityLevel, const int32 InputID) const
{
	// STEP 0) 입력/상태 검증
    if (!ASC)
    {
	    UE_LOG(LogTemp, Warning, TEXT("[ActiveAbilityByTag] ASC is null"));
    	return;
    }
	
    if (!DesiredTag.IsValid())
    {
	    UE_LOG(LogTemp, Warning, TEXT("[ActiveAbilityByTag] DesiredTag is invalid"));
    	return;
    }
    
	// STEP 1) 테이블 찾기
	const UDataTable* AbilityDataTable = nullptr;
	if (const UDataTable* const* Found = AbilityTables.Find(TableKey))
	{
		AbilityDataTable = *Found;
	}
	if (!AbilityDataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("[GiveAbilityByTag] No AbilityTable found for key: %s"), *TableKey.ToString());
		return;
	}

	
	// STEP 2) DataTable에서 태그 매칭
	TArray<FAbilityDataRow*> Rows;
	AbilityDataTable->GetAllRows(TEXT("Ability Lookup"), Rows);
	TSubclassOf<UGameplayAbility> MatchingClass = nullptr;
	
	for (FAbilityDataRow* Row : Rows)
	{
		if (!Row || !Row->AbilityClass) continue;

		const UGameplayAbility* CDO = Row->AbilityClass->GetDefaultObject<UGameplayAbility>();
		if (!CDO) continue;

		if (CDO->GetAssetTags().HasTagExact(DesiredTag))
		{
			MatchingClass = Row->AbilityClass;
			break;
		}
	}
	
	// 매칭 클래스 확인
	if (!MatchingClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("[GiveAbilityByTag] No ability class found for tag: %s"), *DesiredTag.ToString());
		return;
	}

	// STEP 3) 이미 같은 클래스가 있으면 스톱. 중복 부여 방지
	if (FGameplayAbilitySpec* ExistingSpec = ASC->FindAbilitySpecFromClass(MatchingClass); ExistingSpec)
	{
		UE_LOG(LogTemp, Log, TEXT("[GiveAbilityByTag] Already has ability (%s) for tag %s"),*MatchingClass->GetName(), *DesiredTag.ToString());
		return;
	}

	// STEP 4) 권한 확인
	AActor* OwnerActor = ASC->GetOwner();
	if (!(OwnerActor && OwnerActor->HasAuthority()))
	{
		UE_LOG(LogTemp, Warning, TEXT("[GiveAbilityByTag] Not authority; cannot grant ability (%s) for tag %s"),*MatchingClass->GetName(), *DesiredTag.ToString());
		return;
	}
	
    // STEP 5) 새로 부여해야 함. 서버 권한 아니면 종료.
	FGameplayAbilitySpec Spec(MatchingClass, AbilityLevel, (InputID == -1 ? INDEX_NONE : InputID));
	
	if (const FGameplayAbilitySpecHandle Handle = ASC->GiveAbility(Spec); Handle.IsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("[GiveAbilityByTag] Granted ability (%s) for tag %s"),*MatchingClass->GetName(), *DesiredTag.ToString());
	}
	
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[GiveAbilityByTag] Failed to grant ability for tag: %s"),*DesiredTag.ToString());
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UTSAbilityManagerSubSystem::ActiveAbilityByTag(UAbilitySystemComponent* ASC, FGameplayTag DesiredTag, FName TableKey, const int32 AbilityLevel, const int32 InputID) const
{
	// STEP 0) 입력 검증
    if (!ASC)
    {
        UE_LOG(LogTemp, Warning, TEXT("[ActiveAbilityByTag] ASC is null"));
        return;
    }
    if (!DesiredTag.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("[ActiveAbilityByTag] DesiredTag is invalid"));
        return;
    }

    // STEP 1) 이미 부여된 어빌리티 발동 시도
    if (ASC->TryActivateAbilitiesByTag(FGameplayTagContainer(DesiredTag)))
    {
        UE_LOG(LogTemp, Log, TEXT("[ActiveAbilityByTag] Activated existing ability by tag: %s"),*DesiredTag.ToString());
        return;
    }

    // STEP 2) 테이블 찾기
	const UDataTable* AbilityDataTable = nullptr;
    if (const UDataTable* const* Found = AbilityTables.Find(TableKey))
    {
        AbilityDataTable = *Found;
    }
    if (!AbilityDataTable)
    {
        UE_LOG(LogTemp, Warning, TEXT("[ActiveAbilityByTag] No AbilityTable found for key: %s"),*TableKey.ToString());
        return;
    }

    // STEP 3) DataTable에서 태그 매칭
    TArray<FAbilityDataRow*> Rows;
    AbilityDataTable->GetAllRows(TEXT("Ability Lookup"), Rows);
    TSubclassOf<UGameplayAbility> MatchingClass = nullptr;
	
    for (FAbilityDataRow* Row : Rows)
    {
        if (!Row || !Row->AbilityClass) continue;
        const UGameplayAbility* CDO = Row->AbilityClass->GetDefaultObject<UGameplayAbility>();
        if (!CDO) continue;

        if (CDO->AbilityTags.HasTagExact(DesiredTag))
        {
            MatchingClass = Row->AbilityClass;
            break;
        }
    }
    if (!MatchingClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("[ActiveAbilityByTag] No ability class found for tag: %s"),*DesiredTag.ToString());
        return;
    }

    // STEP 4) 이미 같은 클래스 있으면 재활성화
    if (FGameplayAbilitySpec* ExistingSpec = ASC->FindAbilitySpecFromClass(MatchingClass))
    {
        if (ASC->TryActivateAbility(ExistingSpec->Handle))
        {
            UE_LOG(LogTemp, Log, TEXT("[ActiveAbilityByTag] Activated already-granted ability (%s) for tag %s"),*MatchingClass->GetName(), *DesiredTag.ToString());
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("[ActiveAbilityByTag] Found spec but activation failed (%s), tag %s"),*MatchingClass->GetName(), *DesiredTag.ToString());
        }
        return;
    }

    // STEP 5) 서버 권한 확인
    AActor* OwnerActor = ASC->GetOwner();
    if (!(OwnerActor && OwnerActor->HasAuthority()))
    {
        UE_LOG(LogTemp, Warning, TEXT("[ActiveAbilityByTag] Not authority; cannot grant ability (%s) for tag %s"),*MatchingClass->GetName(), *DesiredTag.ToString());
        return;
    }

    // STEP 6) 부여 후 발동
    FGameplayAbilitySpec Spec(MatchingClass, AbilityLevel, (InputID == -1 ? INDEX_NONE : InputID));
    if (const FGameplayAbilitySpecHandle Handle = ASC->GiveAbility(Spec); ASC->TryActivateAbility(Handle))
    {
        UE_LOG(LogTemp, Log, TEXT("[ActiveAbilityByTag] Granted+Activated ability (%s) for tag %s"),*MatchingClass->GetName(), *DesiredTag.ToString());
    }
}

void UTSAbilityManagerSubSystem::ClearAbilityByTag(UAbilitySystemComponent* ASC, FGameplayTag DesiredTag, FName TableKey, const int32 AbilityLevel, const int32 InputID) const
{
	// STEP 0) 입력 검증
    if (!ASC)
    {
        UE_LOG(LogTemp, Warning, TEXT("[ClearAbilityByTag] ASC is null"));
        return;
    }
    if (!DesiredTag.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("[ClearAbilityByTag] DesiredTag is invalid"));
        return;
    }

    // STEP 1) 테이블 찾기
	const UDataTable* AbilityDataTable = nullptr;
    if (const UDataTable* const* Found = AbilityTables.Find(TableKey))
    {
        AbilityDataTable = *Found;
    }

    if (!AbilityDataTable)
    {
        UE_LOG(LogTemp, Warning, TEXT("[ClearAbilityByTag] No AbilityTable found for key: %s"),*TableKey.ToString());
        return;
    }

    // STEP 2) DataTable에서 태그 매칭
    TArray<FAbilityDataRow*> Rows;
    AbilityDataTable->GetAllRows(TEXT("Ability Lookup"), Rows);
    TSubclassOf<UGameplayAbility> MatchingClass = nullptr;

    for (FAbilityDataRow* Row : Rows)
    {
        if (!Row || !Row->AbilityClass) continue;
        const UGameplayAbility* CDO = Row->AbilityClass->GetDefaultObject<UGameplayAbility>();
        if (!CDO) continue;

        if (CDO->AbilityTags.HasTagExact(DesiredTag))
        {
            MatchingClass = Row->AbilityClass;
            break;
        }
    }
    if (!MatchingClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("[ClearAbilityByTag] No ability class found for tag: %s"),*DesiredTag.ToString());
        return;
    }

    // STEP 3) Spec 찾기
    if (FGameplayAbilitySpec* ExistingSpec = ASC->FindAbilitySpecFromClass(MatchingClass))
    {
        // STEP 4) 서버 권한 확인
        AActor* OwnerActor = ASC->GetOwner();
        if (!(OwnerActor && OwnerActor->HasAuthority()))
        {
            UE_LOG(LogTemp, Warning, TEXT("[ClearAbilityByTag] Not authority; cannot clear ability (%s) for tag %s"),*MatchingClass->GetName(), *DesiredTag.ToString());
            return;
        }
        // STEP 5) 제거
        ASC->ClearAbility(ExistingSpec->Handle);
        UE_LOG(LogTemp, Log, TEXT("[ClearAbilityByTag] Cleared ability (%s) for tag %s"),*MatchingClass->GetName(), *DesiredTag.ToString());
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("[ClearAbilityByTag] No granted ability found to clear for tag: %s"),*DesiredTag.ToString());
    }
}
