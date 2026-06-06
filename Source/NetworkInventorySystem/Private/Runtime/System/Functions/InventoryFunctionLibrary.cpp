// Copyright © 2026 bo6eR. All rights reserved.


#include "System/Functions/InventoryFunctionLibrary.h"
#include "Fragments/InventoryItemFragment.h"
#include "System/InventoryUtilityTypes.h"
#include "Components/InventoryComponent.h"
#include "InventoryCoreTypes.h"
#include "Engine/AssetManager.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(InventoryFunctionLibrary)

void UInventoryFunctionLibrary::UnloadAllItems(UObject* Context, EConfirmPolicy& ConfirmPolicy, bool bForceUnload)
{
	checkf(Context, TEXT("World Context Object is nullptr!"));
	
	ConfirmPolicy = EConfirmPolicy::Failure;
	
	if (const UWorld* World = Context->GetWorld())
	{
		if (World->GetNetMode() != NM_Client)
		{
			UAssetManager* AssetManager = UAssetManager::GetIfInitialized();
			check(AssetManager);
	
			if (bForceUnload)
			{
				TArray<FPrimaryAssetId> OutIdList;
				if (AssetManager->GetPrimaryAssetIdList(ItemAssetTypes::Item, OUT OutIdList))
				{
					AssetManager->ChangeBundleStateForPrimaryAssets(OutIdList, TArray<FName> { }, TArray<FName> { }, true);
				}
			}
	
			const int32 Result = AssetManager->UnloadPrimaryAssetsWithType(ItemAssetTypes::Item);
			ConfirmPolicy = Result > 0 ? EConfirmPolicy::Successful : EConfirmPolicy::Failure;
		}
	}
}

TArray<UInventoryItemFragment*> UInventoryFunctionLibrary::GetItemsWithCategoryTag(const FGameplayTag& Tag)
{
	const UAssetManager* AssetManager = &UAssetManager::Get();
	check(AssetManager)
	
	TArray<UInventoryItemFragment*> Result;
	
	TArray<UObject*> Items;
	AssetManager->GetPrimaryAssetObjectList(ItemAssetTypes::Item, Items);
	for (UObject* Item : Items)
	{
		if (const auto ItemClass = Cast<UClass>(Item))
		{
			if (const auto ItemFragment = Cast<UInventoryItemFragment>(ItemClass->GetDefaultObject()))
			{
				if (ItemFragment->CategoryTag == Tag)
				{
					Result.Add(ItemFragment);
				}
			}
		}
	}
	return Result;
}

void UInventoryFunctionLibrary::Trade(UWorld* Context, const TArray<FInventoryItem>& MoveItems, UInventoryComponent* From, UInventoryComponent* To)
{
	checkf(Context, TEXT("Context is nullptr!"));
	
	if (Context->GetNetMode() != NM_Client)
	{
		if (From != nullptr && To != nullptr)
		{
			for (const auto& Item : MoveItems)
			{
				FInventoryInstance ItemData
				(
					Item.ItemFragment->GetPrimaryAssetId(), 
					Context,
					Item.ItemFragment->GetCount()
				);
				To->Server_AddItemAsync(ItemData, true);
				
				if (From->Contains(Item.ItemFragment))
				{
					From->Server_DeleteItemAsync(ItemData);
				}
			}
		}
	}
}
