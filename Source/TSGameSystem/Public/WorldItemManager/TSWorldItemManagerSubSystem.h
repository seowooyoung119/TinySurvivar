#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "DataAsset/TSTables.h"
#include "Subsystems/WorldSubsystem.h"
#include "TSWorldItemManagerSubSystem.generated.h"

/**
 * 
 */
UCLASS()
class TSGAMESYSTEM_API UTSWorldItemManagerSubSystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:
	
	// ========== Lifecycle ==========
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void PostInitialize() override;
	virtual void Deinitialize() override;
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	virtual void BeginDestroy() override;

	// ========== Tick ==========
	virtual void Tick(float DeltaTime) override;
	virtual bool IsTickable() const override { return IsInitialized() && !IsTemplate(); }
	virtual ETickableTickType GetTickableTickType() const override { return ETickableTickType::Always; }
	virtual TStatId GetStatId() const override {RETURN_QUICK_DECLARE_CYCLE_STAT(UTSWorldItemManagerSubSystem, STATGROUP_Tickables);}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

private:
	// 풀 초기화/업데이트/정리
	void InitializePools();
	void UpdatePools();
	void ClearPools();
	
	// 풀 조회/재사용 시도
	AActor* TryPopFromPool(const FGameplayTag& ItemTag, const FVector& Loc, const FRotator& Rot);

	// 풀 반납(비활성화)
	void ReturnToPool(AActor* ItemActor);

	// 스폰 큐 처리
	void ProcessSpawnQueue(int32& InOutSpawnedThisFrame);

	// 비동기 로드 완료 콜백 → 스폰 큐로 전달
	void OnItemClassLoaded(FGameplayTag ItemTag, TSoftClassPtr<AActor> ItemClassSoft);

	// 활성/비활성 토글 헬퍼
	void SetActorActive(AActor* Actor, bool bActive);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

public:
	// ========== Public API ==========
	// 어빌리티에서 호출: 테이블/태그로 아이템 생성 요청 (비동기 로드+스폰 분산 포함)
	UFUNCTION(BlueprintCallable)
	void CalledCreateItem(FName TableKey, FGameplayTag DesiredTag, const FVector& SpawnLocation, const FRotator& SpawnRotation);
	// 어빌리티/게임 로직에서 호출: 아이템 제거 → 풀 반납
	UFUNCTION(BlueprintCallable)
	void CalledDestroyItem(AActor* ItemActor);
	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	

	// 아이템 테이블 (PostInitialize에서 GI로부터 세팅)
	UPROPERTY()
	TMap<FName, UDataTable*> ItemTables;

	// 타입별 풀
	TMap<FGameplayTag, TArray<TWeakObjectPtr<AActor>>> ItemPools;

	// 비동기 로드 대기 / 스폰 대기 큐
	TQueue<FSpawnRequest> PendingLoadQueue;
	TQueue<FSpawnRequest> PendingSpawnQueue;

	// 프레임당 스폰 상한
	int32 MaxSpawnPerTick = 3;
};
