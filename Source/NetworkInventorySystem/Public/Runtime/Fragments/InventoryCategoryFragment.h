// Copyright © 2026 bo6eR. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "NativeGameplayTags.h"
#include "InventoryCategoryFragment.generated.h"

//~ Forwarding declaration
class UInventoryItemFragment;
//~ End of forwarding declaration

UCLASS(BlueprintType)
class NETWORKINVENTORYSYSTEM_API UInventoryCategoryFragment : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Mainline", meta=(Categories="Inventory.Categories"))
	FGameplayTag CategoryTag = FGameplayTag::EmptyTag;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Advanced")
	FName Title = FName(TEXT("Unknown"));
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Advanced")
	TObjectPtr<UTexture2D> Texture = nullptr;
	
private:
	__readonly TSharedPtr<FGuid> Id = MakeShared<FGuid>(FGuid::NewGuid());
};
