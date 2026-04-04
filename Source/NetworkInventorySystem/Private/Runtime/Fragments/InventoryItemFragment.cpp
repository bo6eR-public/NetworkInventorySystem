// Copyright © 2026 bo6eR. All rights reserved.


#include "Fragments/InventoryItemFragment.h"
#include "System/Settings/InventoryGameSettings.h"
#include "Net/UnrealNetwork.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(InventoryItemFragment)

void UInventoryItemFragment::OnRep_Count_Implementation()
{
	// Kind of reflection...
}

void UInventoryItemFragment::OnRep_Texture_Implementation()
{
	// Kind of reflection...
}

void UInventoryItemFragment::OnRep_Mesh_Implementation()
{
	// Kind of reflection...
}

void UInventoryItemFragment::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ThisClass, Mesh);
	DOREPLIFETIME(ThisClass, Texture);
	DOREPLIFETIME_CONDITION(ThisClass, Count, COND_OwnerOnly);
}

bool UInventoryItemFragment::IsCategorized() const
{
	if (const auto InventorySettings = UInventoryGameSettings::Get())
	{
		return InventorySettings->IsCategorized();
	}
	return false;
}

FPrimaryAssetId UInventoryItemFragment::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(AssetType, GetFName());
}

const UInventoryCategoryFragment* UInventoryItemFragment::GetCategoryByTag() const
{
	if (const auto InventorySettings = UInventoryGameSettings::Get())
	{
		if (InventorySettings->IsCategorized())
		{
			return InventorySettings->GetCategoryByTag(CategoryTag);
		}
	}
	return nullptr;
}
