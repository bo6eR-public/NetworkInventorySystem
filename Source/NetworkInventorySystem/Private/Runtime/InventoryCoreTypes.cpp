// Copyright © 2026 bo6eR. All rights reserved.

#pragma once

#include "InventoryCoreTypes.h"
#include "Components/InventoryComponent.h"
#include "Fragments/InventoryItemFragment.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(InventoryCoreTypes)

FInventoryItem& FInventoryArray::AddItem(TObjectPtr<UInventoryItemFragment> ItemFragment)
{
	check(OwningComponent);
	check(OwningComponent->GetOwner()->HasAuthority());
	
	if (ItemFragment->IsValidLowLevelFast())
	{
		FInventoryItem& Item = Items.AddDefaulted_GetRef();
		Item.ItemFragment = ItemFragment;
		
		MarkItemDirty(Item);
		return Item;
	}
	return FInventoryItem::GetDefaultObject(Error);
}

void FInventoryArray::RemoveItem(const FInventoryItem& Item)
{
	check(OwningComponent);
	check(OwningComponent->GetOwner()->HasAuthority());
	
	for (auto It = Items.CreateIterator(); It; ++It)
	{
		FInventoryItem& Elem = *It;
		if (Elem.IsValid() && Elem.ItemFragment == Item.ItemFragment)
		{
			It.RemoveCurrent();
			MarkArrayDirty();
		}
	}
}

void FInventoryItem::PreReplicatedRemove(const FInventoryArray& InArraySerializer)
{
	if (const auto InventoryComponent = InArraySerializer.GetOwningComponent())
	{
		InventoryComponent->PreReplicatedRemoveDelegate.Broadcast(InArraySerializer);
	}
}

void FInventoryItem::PostReplicatedAdd(const FInventoryArray& InArraySerializer)
{
	if (const auto InventoryComponent = InArraySerializer.GetOwningComponent())
	{
		InventoryComponent->PostReplicatedAddDelegate.Broadcast(InArraySerializer);
	}
}

void FInventoryItem::PostReplicatedChange(const FInventoryArray& InArraySerializer)
{
	if (const auto InventoryComponent = InArraySerializer.GetOwningComponent())
	{
		InventoryComponent->PostReplicatedChangeDelegate.Broadcast(InArraySerializer);
	}
}

bool FInventoryItem::Update(EUpdateCommand UpdateCommand, int32 Quantity) const
{
	switch (UpdateCommand)
	{
	case EUpdateCommand::INCREASE:
		ItemFragment->SetCount(ItemFragment->GetCount() + Quantity);
		break;
	case EUpdateCommand::DECREASE:
		ItemFragment->SetCount(ItemFragment->GetCount() - Quantity);
		if (!ItemFragment->GetCount()) return false;
		break;
	}
	return true;
}

FInventoryArray::FInventoryArray(UInventoryComponent* InOwningComponent, int32 Reserve) : OwningComponent(InOwningComponent)
{
	Items.Reserve(Reserve);
}

namespace InventoryGameplayTags
{
	NETWORKINVENTORYSYSTEM_API UE_DEFINE_GAMEPLAY_TAG(Inventory_Categories, "Inventory.Categories");
}
