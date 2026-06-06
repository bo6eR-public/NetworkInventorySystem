// Copyright © 2026 bo6eR. All rights reserved.


#include "System/InventoryUtilityTypes.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(InventoryUtilityTypes)

FInventoryInstance::FInventoryInstance(const FPrimaryAssetId& InPrimaryAssetId, UObject* InOwner, int32 InQuantity) : PrimaryAssetId(InPrimaryAssetId), Quantity(InQuantity)
{
	ensureMsgf(GEngine, TEXT("GEngine is not initialized!"));
	Owner = InOwner != nullptr ? InOwner : GEngine->GetCurrentPlayWorld();
}
