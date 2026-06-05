// Copyright © 2026 bo6eR. All rights reserved.


#include "Fragments/InventoryItemFragment.h"
#include "System/Settings/InventoryGameSettings.h"
#include "Net/Core/PushModel/PushModel.h"
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
	
	FDoRepLifetimeParams OutLifetimeProp;
	OutLifetimeProp.bIsPushBased = true;
	
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, Mesh, OutLifetimeProp);
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, Texture, OutLifetimeProp);
	
	FDoRepLifetimeParams OutLifetimePropWithCondition;
	OutLifetimePropWithCondition.bIsPushBased = true;
	OutLifetimePropWithCondition.Condition = COND_OwnerOnly;
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, Count, OutLifetimePropWithCondition);
}

bool UInventoryItemFragment::IsCategorized() const
{
	if (const auto InventorySettings = UInventoryGameSettings::Get())
	{
		return InventorySettings->IsCategorized();
	}
	return false;
}

void UInventoryItemFragment::SetCount(int32 NewCount)
{
	Count = FMath::Max(0, NewCount);
	MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, Count, this);
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
