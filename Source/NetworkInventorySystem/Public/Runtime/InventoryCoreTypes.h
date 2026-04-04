// Copyright © 2026 bo6eR. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"
#include "Fragments/InventoryItemFragment.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "InventoryCoreTypes.generated.h"

//~ Forwarding declaration
class UInventoryItemFragment;
struct FInventoryArray;
struct FStreamableHandle;
//~ End of forwarding declaration

UENUM()
enum EMessageFlag
{
	Ok = 0,
	Warning = 1 << 0,
	Error = 1 << 1,
};
ENUM_CLASS_FLAGS(EMessageFlag)

UENUM()
enum class EUpdateCommand : uint8
{
	INCREASE UMETA(DisplayName = "Increase"),
	DECREASE UMETA(DisplayName = "Decrease"),
};
ENUM_CLASS_FLAGS(EUpdateCommand)

USTRUCT(BlueprintType)
struct FInventoryItem : public FFastArraySerializerItem
{
	GENERATED_BODY()
	
public:
	explicit FInventoryItem(const EMessageFlag& Message = Ok)
	{
		bIsValid = Message != Error;
	}
	
	FORCEINLINE bool IsValid() const { return bIsValid && ItemFragment; }
	
	//~ Begin of FFastArraySerializerItem Interface
	void PreReplicatedRemove(const FInventoryArray& InArraySerializer);
	void PostReplicatedAdd(const FInventoryArray& InArraySerializer);
	void PostReplicatedChange(const FInventoryArray& InArraySerializer);
	//~ End of FFastArraySerializerItem Interface
	
	bool Update(EUpdateCommand UpdateCommand, int32 Quantity) const;
	
	FORCEINLINE static FInventoryItem& GetDefaultObject(const EMessageFlag& Message)
	{
		static FInventoryItem DefaultItem(Message); return DefaultItem;
	}
	
	bool operator==(const FInventoryItem& Entry) const
	{
		return this->ItemFragment->GetPrimaryAssetId() == Entry.ItemFragment->GetPrimaryAssetId();
	}
	
public:
	UPROPERTY(BlueprintReadOnly, Category=Item)
	TObjectPtr<UInventoryItemFragment> ItemFragment;
	
private:
	UPROPERTY(Transient)
	bool bIsValid = true;
};

USTRUCT(BlueprintType)
struct FInventoryArray : public FFastArraySerializer
{
	GENERATED_BODY()
	
public:
	FInventoryArray() { }
	explicit FInventoryArray(UInventoryComponent* InOwningComponent, int32 Reserve = 256);
	
	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParams)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FInventoryItem, FInventoryArray>( Items, DeltaParams, *this);
	}
	
	bool IsOutOfBounds(int32 Index) const { return Index < 0 || Index >= Items.Num(); }
	
	FInventoryItem& operator[](const int32 Index)
	{
		if (!IsOutOfBounds(Index))
		{
			return Items[Index];
		}
		return FInventoryItem::GetDefaultObject(Error);
	}
	
	template<typename Predicate>
	const FInventoryItem* FindByPredicate(Predicate Pred) const
	{
		return Items.FindByPredicate(Pred);
	}
	template<typename Predicate>
	FORCEINLINE bool ContainsByPredicate(Predicate Pred) const
	{
		return Items.ContainsByPredicate(Pred);
	}
	
	FInventoryItem& AddItem(TObjectPtr<UInventoryItemFragment> ItemFragment);
	void RemoveItem(const FInventoryItem& Item);
	
	FORCEINLINE bool IsEmpty() const { return !Items.Num(); }
	
	/* Returns the FInventoryItem array */
	FORCEINLINE TArray<FInventoryItem> Get() const { return Items; }
	FORCEINLINE UInventoryComponent* GetOwningComponent() const { return OwningComponent; }

private:
	UPROPERTY()
	TArray<FInventoryItem> Items;
	
	UPROPERTY()
	TObjectPtr<UInventoryComponent> OwningComponent = nullptr;
};

template<>
struct TStructOpsTypeTraits< FInventoryArray > : public TStructOpsTypeTraitsBase2< FInventoryArray >
{
	enum { WithNetDeltaSerializer = true };
};

namespace InventoryGameplayTags
{
	NETWORKINVENTORYSYSTEM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Inventory_Categories);
}
