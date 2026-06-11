// Copyright © 2026 bo6eR. All rights reserved.


#include "Components/InventoryComponent.h"
#include "Fragments/InventoryItemFragment.h"
#include "Net/Core/PushModel/PushModel.h"
#include "System/InventoryUtilityTypes.h"
#include "Engine/ActorChannel.h"
#include "Engine/AssetManager.h"
#include "Net/UnrealNetwork.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(InventoryComponent)

UInventoryComponent::UInventoryComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer), InventoryArray(this)
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	
	bWantsInitializeComponent = true;
	SetIsReplicatedByDefault(true);
}

TArray<FInventoryItem> UInventoryComponent::GetItems() const
{
	TArray<FInventoryItem> OutEntries;
	for (const FInventoryItem& Entry : InventoryArray.Get())
	{
		if (Entry.IsValid())
		{
			OutEntries.Add(Entry);
		}
	}
	return OutEntries;
}

FInventoryItem& UInventoryComponent::GetItemRefAt(const int32 Index)
{
	return InventoryArray[Index];
}

FInventoryItem UInventoryComponent::GetItemCopyAt(const int32 Index)
{
	return InventoryArray[Index];
}

void UInventoryComponent::Server_AddItemAsync_Implementation(const FInventoryInstance& InventoryDataInfo, bool bForceLoad)
{
	UAssetManager* AssetManager = UAssetManager::GetIfInitialized();
	check(AssetManager);
	
	const FPrimaryAssetId PrimaryAssetId = InventoryDataInfo.PrimaryAssetId;
	
	auto FoundationCondition = [&](const FInventoryItem& Item)
	{
		return Item.IsValid() && Item.ItemFragment->GetPrimaryAssetId() == PrimaryAssetId;
	};
	if (const FInventoryItem* FoundItem = InventoryArray.FindByPredicate(FoundationCondition))
	{
		const bool bResult = FoundItem->Update(EUpdateCommand::INCREASE, InventoryDataInfo.Quantity);
		if (!bResult)
		{
			return;
		}
	}
	
	TArray<FName> Bundles;
	/* bForceLoad provides loading all bundles that Fragment owns */
	if (bForceLoad)
	{
		TArray<FAssetBundleEntry> OutEntries;
		if (AssetManager->GetAssetBundleEntries(PrimaryAssetId, OUT OutEntries))
		{
			for (auto Entry : OutEntries)
			{
				Bundles.Add(Entry.BundleName);
			}
		}
	}
	
	PostLoadHandle = AssetManager->LoadPrimaryAsset(PrimaryAssetId, Bundles, FStreamableDelegate::CreateLambda([this]
	{
		if (PostLoadHandle.IsValid() && PostLoadHandle->HasLoadCompleted())
		{
			if (const auto PostLoadObject = PostLoadHandle->GetLoadedAsset<UClass>())
			{
				if (const auto ItemFragment = Cast<UInventoryItemFragment>(PostLoadObject->GetDefaultObject()))
				{
					InventoryArray.AddItem(ItemFragment);
					OnInventoryUpdate.Broadcast();
				}
			}
		}
	}));
	MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, InventoryArray, this);
}

void UInventoryComponent::Server_DeleteItemAsync_Implementation(const FInventoryInstance& InventoryDataInfo)
{
	UAssetManager* AssetManager = &UAssetManager::Get();
	check(AssetManager);
	
	const FInventoryItem* Item = InventoryArray.FindByPredicate([&](const FInventoryItem& Entry)
	{
		return Entry.ItemFragment->GetPrimaryAssetId() == InventoryDataInfo.PrimaryAssetId;
	});
	if (Item->IsValid())
	{
		if (!Item->Update(EUpdateCommand::DECREASE, InventoryDataInfo.Quantity))
		{
			const FPrimaryAssetId& PrimaryAssetId = Item->ItemFragment->GetPrimaryAssetId();
			if (PrimaryAssetId.IsValid())
			{
				if (AssetManager->UnloadPrimaryAsset(PrimaryAssetId))
				{
					InventoryArray.RemoveItem(*Item);
					OnInventoryUpdate.Broadcast();
				}
			}
		}
	}
	if (InventoryArray.IsEmpty())
	{
		OnInventoryEmpty.Broadcast();
	}
	MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, InventoryArray, this);
}

bool UInventoryComponent::Contains(const UInventoryItemFragment* EntryFragment) const
{
	for (const FInventoryItem& Entry : InventoryArray.Get())
	{
		if (Entry.IsValid())
		{
			if (Entry.ItemFragment == EntryFragment)
			{
				return true;
			}
		}
	}
	return false;
}

bool UInventoryComponent::ContainsById(const FPrimaryAssetId& PrimaryAssetId) const
{
	for (const FInventoryItem& Entry : InventoryArray.Get())
	{
		if (Entry.IsValid())
		{
			if (Entry.ItemFragment->GetPrimaryAssetId() == PrimaryAssetId)
			{
				return true;
			}
		}
	}
	return false;
}

const FInventoryItem& UInventoryComponent::FindBy(const UInventoryItemFragment* EntryFragment) const
{
	for (const FInventoryItem& Entry : InventoryArray.Get())
	{
		if (Entry.IsValid())
		{
			if (Entry.ItemFragment == EntryFragment)
			{
				return Entry;
			}
		}
	}
	return FInventoryItem::GetDefaultObject(Error);
}

void UInventoryComponent::BP_SortItems(TArray<FInventoryItem>& Sorted, EInventorySortOrder SortOrder, const FName ParamName) const
{
	checkNoEntry();
}

DEFINE_FUNCTION(UInventoryComponent::execBP_SortItems)
{
	PARAM_PASSED_BY_REF(ITEMS_ARRAY, FArrayProperty, TArray<FInventoryItem>);
	PARAM_PASSED_BY_VAL(SORT_ORDER, FEnumProperty, EInventorySortOrder);
	PARAM_PASSED_BY_VAL(SORT_PARAM_NAME, FNameProperty, FName);
	
	P_FINISH;
	
	P_NATIVE_BEGIN;
	
	TArray<FInventoryItem> Sorted = P_THIS->GetItems();
	
	if (SORT_PARAM_NAME == GET_MEMBER_NAME_CHECKED(UInventoryItemFragment, Count))
	{
		Sorted.Sort([&](const FInventoryItem& A, const FInventoryItem& B)
		{
			return SORT_ORDER == EInventorySortOrder::Ascending ? A.ItemFragment->GetCount() > B.ItemFragment->GetCount() : A.ItemFragment->GetCount() < B.ItemFragment->GetCount();
		});
	}
	else if (SORT_PARAM_NAME == GET_MEMBER_NAME_CHECKED(UInventoryItemFragment, Name))
	{
		Sorted.Sort([&](const FInventoryItem& A, const FInventoryItem& B)
		{
			return SORT_ORDER == EInventorySortOrder::Ascending ? A.ItemFragment->GetItemName().ToString() > B.ItemFragment->GetItemName().ToString() : A.ItemFragment->GetItemName().ToString() < B.ItemFragment->GetItemName().ToString();
		});
	}
	else if (SORT_PARAM_NAME == GET_MEMBER_NAME_CHECKED(UInventoryItemFragment, bIsStackable))
	{
		Sorted.Sort([&](const FInventoryItem& A, const FInventoryItem& B)
		{
			return SORT_ORDER == EInventorySortOrder::Ascending ? A.ItemFragment->GetIsStackable() > B.ItemFragment->GetIsStackable() : A.ItemFragment->GetIsStackable() < B.ItemFragment->GetIsStackable();
		});
	}
	ITEMS_ARRAY = Sorted;
	
	P_NATIVE_END;
}

void UInventoryComponent::BP_FilterItems(TArray<FInventoryItem>& Filtered, FGameplayTagContainer CategoriesTagContainer, EMatchType MatchType) const
{
	checkNoEntry();
}

DEFINE_FUNCTION(UInventoryComponent::execBP_FilterItems)
{
	PARAM_PASSED_BY_REF(ITEMS_ARRAY, FArrayProperty, TArray<FInventoryItem>);
	PARAM_PASSED_BY_VAL(FILTER_GAMEPLAY_TAG_CONTAINER, FStructProperty, FGameplayTagContainer);
	PARAM_PASSED_BY_VAL(MATCH_TYPE, FEnumProperty, EMatchType);
	
	P_FINISH;
	
	P_NATIVE_BEGIN;
	
	TArray<FInventoryItem> Filtered = P_THIS->GetItems();
	for (const FGameplayTag& FilterTag : FILTER_GAMEPLAY_TAG_CONTAINER.GetGameplayTagArray())
	{
		if (MATCH_TYPE == EMatchType::PartialMatch)
		{
			Filtered = Filtered.FilterByPredicate([FilterTag](const FInventoryItem& A)
			{
				if (A.IsValid())
				{
					for (FGameplayTag Tag = A.ItemFragment->CategoryTag; Tag.IsValid(); Tag = Tag.RequestDirectParent())
					{
						if (Tag == FilterTag)
						{
							return true;
						}
					}
				}
				return false;
			});
		}
		else
		{
			Filtered = Filtered.FilterByPredicate([FilterTag](const FInventoryItem& A)
			{
				return A.IsValid() && A.ItemFragment->CategoryTag == FilterTag;
			});
		}
	}
	ITEMS_ARRAY = Filtered;
	
	P_NATIVE_END;
}

template<typename Predicate>
void UInventoryComponent::Sort_Internal(TArray<FInventoryItem>& Sorted, Predicate Function)
{
	if (GetOwner() && !GetOwner()->HasAuthority())
	{
		if (Sorted = InventoryArray.Get(); Sorted.Num())
		{
			Sorted.Sort(Function);
		}
	}
}

template<typename Predicate>
void UInventoryComponent::Filter_Internal(TArray<FInventoryItem>& Filtered, Predicate Function)
{
	if (GetOwner() && !GetOwner()->HasAuthority())
	{
		if (TArray Items { InventoryArray.Get() }; Items.Num())
		{
			Filtered = Items.FilterByPredicate(Function);
		}
	}
}

void UInventoryComponent::Server_ClearInventory_Implementation()
{
	UAssetManager* AssetManager = &UAssetManager::Get();
	check(AssetManager);
	
	for (const FInventoryItem& Item : GetItems())
	{
		if (Item.IsValid())
		{
			AssetManager->UnloadPrimaryAsset(Item.ItemFragment->GetPrimaryAssetId());
		}
	}
	InventoryArray.Get().Empty();
	OnInventoryEmpty.Broadcast();
	MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, InventoryArray, this);
}

void UInventoryComponent::PrintInventoryItems(const TArray<FInventoryItem>& Items, float TimeToDisplay, FLinearColor Color)
{
#if !UE_BUILD_SHIPPING
	check(GEngine);
	for (const FInventoryItem& Item : Items)
	{
		if (const auto Fragment = Item.ItemFragment)
		{
			const FString ItemInfo = FString::Printf(TEXT("Name: %s,\nDescription: %s,\nCount: %d\n"), *Fragment->GetItemName().ToString(), *Fragment->GetDescription(), Fragment->GetCount());
			GEngine->AddOnScreenDebugMessage(INDEX_NONE, TimeToDisplay, Color.ToFColor(true), ItemInfo);
		}
	}
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, TimeToDisplay, Color.ToFColor(true), FString::Printf(TEXT("Inventory Items:\n")));
#endif
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	FDoRepLifetimeParams OutLifetimeProp;
	OutLifetimeProp.bIsPushBased = true;
	OutLifetimeProp.Condition = COND_OwnerOnly;
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, InventoryArray, OutLifetimeProp);
}

bool UInventoryComponent::ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool bWroteSomething =  Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	for (const FInventoryItem& Item : InventoryArray.Get())
	{
		if (Item.ItemFragment)
		{
			bWroteSomething |= Channel->ReplicateSubobject(Item.ItemFragment, *Bunch, *RepFlags);
		}
	}
	return bWroteSomething;
}

void UInventoryComponent::ReadyForReplication()
{
	Super::ReadyForReplication();
	if (IsUsingRegisteredSubObjectList())
	{
		for (const FInventoryItem& Item : InventoryArray.Get())
		{
			if (Item.ItemFragment)
			{
				AddReplicatedSubObject(Item.ItemFragment);
			}
		}
	}
}

void UInventoryComponent::OnRep_InventoryArray_Implementation()
{
	// Kind of reflection...
}
