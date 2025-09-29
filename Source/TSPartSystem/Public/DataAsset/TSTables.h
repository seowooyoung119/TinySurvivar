#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "TSTables.generated.h"

USTRUCT(BlueprintType)
struct FItemRow : public FTableRowBase
{
	GENERATED_BODY()

	// 아이템 식별용 태그
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	FGameplayTag ItemTag;

	// 실제 스폰할 아이템 클래스
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	TSoftClassPtr<AActor> ItemClassSoft;
};

// 스폰 요청 (비동기 로드 + 분산 스폰을 잇는 데이터)
USTRUCT()
struct FSpawnRequest
{
	GENERATED_BODY()

	UPROPERTY()
	FGameplayTag ItemTag;

	UPROPERTY()
	TSoftClassPtr<AActor> ItemClassSoft;

	UPROPERTY()
	FVector Location = FVector::ZeroVector;

	UPROPERTY()
	FRotator Rotation = FRotator::ZeroRotator;

	// 풀 확장(비활성 스폰)인지, 실제 플레이용(활성 스폰)인지 구분
	UPROPERTY()
	bool bActivateOnSpawn = true;

	FSpawnRequest() {}
	FSpawnRequest(TSoftClassPtr<AActor> InSoft, const FVector& InLoc, const FRotator& InRot, FGameplayTag InTag, bool bActivate)
		: ItemTag(InTag), ItemClassSoft(InSoft), Location(InLoc), Rotation(InRot), bActivateOnSpawn(bActivate) {}
};
