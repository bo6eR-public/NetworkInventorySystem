// Copyright © 2026 bo6eR. All rights reserved.

#pragma once

#include "InventoryUtilityTypes.generated.h"

USTRUCT(BlueprintType, Blueprintable)
struct FInventoryDataInfo
{
	GENERATED_BODY()
	
public:
	explicit FInventoryDataInfo() = default;
	explicit FInventoryDataInfo(const FPrimaryAssetId& InPrimaryAssetId, UObject* InOwner = nullptr, int32 InQuantity = 1);

	template<class T>
	inline T* GetWeakOwner() const { return Cast<T>(Owner); }
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Shared)
	FPrimaryAssetId PrimaryAssetId;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Advanced)
	int32 Quantity = 1;
	
private:
	/* It provides the owner container for different cases */
	TWeakObjectPtr<UObject> Owner = nullptr;
};

UENUM(BlueprintType)
enum class EConfirmPolicy : uint8
{
	Successful,
	Failure
};

UENUM()
enum class EInventorySortOrder : uint8
{
	Ascending,
	Descending
};

UENUM(BlueprintType)
enum class EMatchType : uint8
{
	ExactMatch,
	PartialMatch
};
