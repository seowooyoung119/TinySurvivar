#include "WorldItemManager/TSWorldItemManagerSubSystem.h"
#include "DataAsset/TSAbilityDataAsset.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "GameInstance/TSGameInstance.h"
#include "TSPartSystem/Public/DataAsset/TSItemDataAsset.h"

// ========== Lifecycle ==========

void UTSWorldItemManagerSubSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UE_LOG(LogTemp, Warning, TEXT("WorldItemManager Initialized: %s"), *GetName());
}

void UTSWorldItemManagerSubSystem::PostInitialize()
{
	Super::PostInitialize();

	//  아이템 매니저 초기화
	UTSGameInstance* GI = GetWorld() ? GetWorld()->GetGameInstance<UTSGameInstance>() : nullptr;
	if (!GI)
	{
		UE_LOG(LogTemp, Warning, TEXT("WorldItemManager: No TSGameInstance"));
		return;
	}

	if (!GI->ItemAsset || GI->ItemAsset->ItemTables.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("WorldItemManager: No ItemAsset or ItemTables"));
		return;
	}

	ItemTables = GI->ItemAsset->ItemTables;
	for (const auto& Pair : ItemTables)
	{
		UE_LOG(LogTemp, Log, TEXT("WorldItemManager: Loaded ItemTable [%s]"), *Pair.Key.ToString());
	}
}

void UTSWorldItemManagerSubSystem::Deinitialize()
{
	Super::Deinitialize();
	UE_LOG(LogTemp, Warning, TEXT("WorldItemManager Deinitialized: %s"), *GetName());
}

void UTSWorldItemManagerSubSystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	InitializePools();
}

void UTSWorldItemManagerSubSystem::BeginDestroy()
{
	ClearPools();
	Super::BeginDestroy();
}

// ========== Tick ==========

void UTSWorldItemManagerSubSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (IsInitialized())
	{
		int32 SpawnedThisFrame = 0;
		ProcessSpawnQueue(SpawnedThisFrame);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ========== Public API ==========

void UTSWorldItemManagerSubSystem::CalledCreateItem(FName TableKey, FGameplayTag DesiredTag, const FVector& SpawnLocation, const FRotator& SpawnRotation)
{
	// 0) 풀에서 재사용 시도
	if (AActor* Reused = TryPopFromPool(DesiredTag, SpawnLocation, SpawnRotation))
	{
		UE_LOG(LogTemp, Log, TEXT("[CalledCreateItem] Reused from pool: %s (Tag=%s)"),*GetNameSafe(Reused), *DesiredTag.ToString());
		return;
	}

	// 1) 테이블 찾기
	const UDataTable* ItemDataTable = nullptr;
	if (const UDataTable* const* Found = ItemTables.Find(TableKey))
	{
		ItemDataTable = *Found;
	}
	if (!ItemDataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("[CalledCreateItem] No ItemTable for key: %s"), *TableKey.ToString());
		return;
	}

	// 2) 태그 → 소프트 클래스 찾기
	TArray<FItemRow*> Rows;
	ItemDataTable->GetAllRows(TEXT("Item Lookup"), Rows);

	TSoftClassPtr<AActor> ItemClassSoft = nullptr;
	for (FItemRow* Row : Rows)
	{
		if (Row && Row->ItemTag == DesiredTag)
		{
			ItemClassSoft = Row->ItemClassSoft;
			break;
		}
	}
	if (ItemClassSoft.IsNull())
	{
		UE_LOG(LogTemp, Warning, TEXT("[CalledCreateItem] No class for tag: %s"), *DesiredTag.ToString());
		return;
	}

	// 3) 이미 로드되어 있으면 스폰 큐에 즉시 투입, 아니면 비동기 로드 후 콜백에서 큐 투입
	if (ItemClassSoft.IsValid())
	{
		PendingSpawnQueue.Enqueue(FSpawnRequest(ItemClassSoft, SpawnLocation, SpawnRotation, DesiredTag, /*bActivateOnSpawn=*/true));
		UE_LOG(LogTemp, Log, TEXT("[CalledCreateItem] Enqueued (already loaded): %s (Tag=%s)"),*ItemClassSoft.Get()->GetName(), *DesiredTag.ToString());
	}
	else
	{
		PendingLoadQueue.Enqueue(FSpawnRequest(ItemClassSoft, SpawnLocation, SpawnRotation, DesiredTag, /*bActivateOnSpawn=*/true));

		FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
		Streamable.RequestAsyncLoad(ItemClassSoft.ToSoftObjectPath(),FStreamableDelegate::CreateUObject(this, &UTSWorldItemManagerSubSystem::OnItemClassLoaded, DesiredTag, ItemClassSoft));

		UE_LOG(LogTemp, Log, TEXT("[CalledCreateItem] RequestAsyncLoad: %s (Tag=%s)"),*ItemClassSoft.ToString(), *DesiredTag.ToString());
	}
}

void UTSWorldItemManagerSubSystem::CalledDestroyItem(AActor* ItemActor)
{
	if (!ItemActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("[CalledDestroyItem] ItemActor is null"));
		return;
	}

	// Destroy 대신 풀 반납
	ReturnToPool(ItemActor);
	UE_LOG(LogTemp, Log, TEXT("[CalledDestroyItem] Returned to pool: %s"), *GetNameSafe(ItemActor));
}

// ========== Pools (hooks) ==========

void UTSWorldItemManagerSubSystem::InitializePools()
{
	// 필요 시 맵 시작 시 프리워밍 로직 배치
	// 예) 주요 태그 10개씩 미리 비활성 스폰 (bActivateOnSpawn=false) 큐에 투입
}

void UTSWorldItemManagerSubSystem::UpdatePools()
{
	// 필요 시 주기적 Trim/확장 정책 배치
}

void UTSWorldItemManagerSubSystem::ClearPools()
{
	// 풀 정리 (비활성 인스턴스 Destroy 등)
	for (auto& Pair : ItemPools)
	{
		auto& Arr = Pair.Value;
		for (int32 i = Arr.Num() - 1; i >= 0; --i)
		{
			if (AActor* Actor = Arr[i].Get())
			{
				Actor->Destroy();
			}
		}
		Arr.Empty();
	}
	ItemPools.Empty();
}

// ========== Internals ==========

AActor* UTSWorldItemManagerSubSystem::TryPopFromPool(const FGameplayTag& ItemTag, const FVector& Loc, const FRotator& Rot)
{
	TArray<TWeakObjectPtr<AActor>>* PoolPtr = ItemPools.Find(ItemTag);
	if (!PoolPtr) return nullptr;

	for (int32 i = 0; i < PoolPtr->Num(); ++i)
	{
		if (AActor* Actor = (*PoolPtr)[i].Get())
		{
			// 비활성 상태라고 가정: HiddenInGame=true, Collision Off, Tick Off 등
			if (Actor->IsHidden())
			{
				Actor->SetActorLocationAndRotation(Loc, Rot);
				SetActorActive(Actor, /*bActive=*/true);
				return Actor;
			}
		}
	}
	return nullptr;
}

void UTSWorldItemManagerSubSystem::ReturnToPool(AActor* ItemActor)
{
	if (!ItemActor) return;

	// 태그를 알아야 풀 키로 되돌릴 수 있음 → 방법 2가지:
	// 1) AActor에 ItemTag를 UPROPERTY로 들고 있게 하기
	// 2) 이름 규칙/컴포넌트 등으로 역추적
	// 여기선 간단히 "첫 번째 풀 배열"에라도 넣되, 실전에서는 반드시 태그를 붙여주세요.

	// ★ 권장: AItemBase를 만들어 FGameplayTag ItemTag 보관하고, 여기서 캐스팅해서 꺼내기.
	// 예시용으로는 Hidden 처리만 수행:
	SetActorActive(ItemActor, /*bActive=*/false);

	// (선택) 적절한 태그 풀에 넣기: 실제 구현에서는 정확한 태그 키로 관리하세요.
	// ItemPools.FindOrAdd(ThatTag).Add(ItemActor); 
}

void UTSWorldItemManagerSubSystem::ProcessSpawnQueue(int32& InOutSpawnedThisFrame)
{
	UWorld* World = GetWorld();
	if (!World) return;

	while (!PendingSpawnQueue.IsEmpty() && InOutSpawnedThisFrame < MaxSpawnPerTick)
	{
		FSpawnRequest Req;
		PendingSpawnQueue.Dequeue(Req);

		UClass* LoadedClass = Req.ItemClassSoft.Get();
		if (!LoadedClass)
		{
			// 이 프레임에 아직 로드 미완 → 다음 틱에서 다시 시도되도록 로드 큐에 재투입
			PendingLoadQueue.Enqueue(Req);
			continue;
		}

		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		AActor* NewItem = World->SpawnActor<AActor>(LoadedClass, Req.Location, Req.Rotation, Params);
		if (NewItem)
		{
			if (Req.bActivateOnSpawn)
			{
				SetActorActive(NewItem, /*bActive=*/true);
				UE_LOG(LogTemp, Log, TEXT("[Tick] Spawned ACTIVE item (%s) Tag=%s"),*GetNameSafe(NewItem), *Req.ItemTag.ToString());
			}
			else
			{
				SetActorActive(NewItem, /*bActive=*/false);
				ItemPools.FindOrAdd(Req.ItemTag).Add(NewItem);
				UE_LOG(LogTemp, Log, TEXT("[Tick] Spawned POOLED item (%s) Tag=%s"),*GetNameSafe(NewItem), *Req.ItemTag.ToString());
			}
		}

		++InOutSpawnedThisFrame;
	}
}

void UTSWorldItemManagerSubSystem::OnItemClassLoaded(FGameplayTag ItemTag, TSoftClassPtr<AActor> ItemClassSoft)
{
	// 로드 대기 큐에서 해당 클래스 요청을 찾아 스폰 큐로 넘김
	// (여기서는 단순화: 들어온 것 그대로 스폰 큐로 이동)
	FSpawnRequest Moved;
	// 실제로는 PendingLoadQueue에서 조건에 맞는 요청을 찾아야 함.
	// 간단화: 같은 SoftClass를 가진 첫 항목을 하나 꺼내 사용 (여러 개면 여러 번 콜백됨)
	bool bFound = false;
	{
		TQueue<FSpawnRequest> Temp;
		while (!PendingLoadQueue.IsEmpty())
		{
			FSpawnRequest R;
			PendingLoadQueue.Dequeue(R);
			if (!bFound && R.ItemClassSoft == ItemClassSoft && R.ItemTag == ItemTag)
			{
				Moved = R;
				bFound = true;
			}
			else
			{
				Temp.Enqueue(R);
			}
		}
		// 남은 것 환원
		while (!Temp.IsEmpty())
		{
			FSpawnRequest R; Temp.Dequeue(R);
			PendingLoadQueue.Enqueue(R);
		}
	}

	if (bFound)
	{
		PendingSpawnQueue.Enqueue(Moved);
		UE_LOG(LogTemp, Log, TEXT("[OnItemClassLoaded] Move to SpawnQueue: %s (Tag=%s)"),*ItemClassSoft.ToString(), *ItemTag.ToString());
	}
}

void UTSWorldItemManagerSubSystem::SetActorActive(AActor* Actor, bool bActive)
{
	if (!Actor) return;

	Actor->SetActorHiddenInGame(!bActive);
	Actor->SetActorEnableCollision(bActive);
	Actor->SetActorTickEnabled(bActive);
	// 필요 시 컴포넌트 단위로도 켜고 끄기
}