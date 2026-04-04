// Copyright © 2026 bo6eR. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "InventoryFunctionLibrary.generated.h"

//~ Forwarding declaration
class UInventoryItemFragment;
class UInventoryComponent;
struct FGameplayTag;
struct FInventoryItem;
enum class EConfirmPolicy : uint8;
//~ End of forwarding declaration

UCLASS()
class NETWORKINVENTORYSYSTEM_API UInventoryFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, meta=(WorldContext="Context", ExpandEnumAsExecs="ConfirmPolicy"), DisplayName="Force Unload All Inventory Items", Category=Functionality)
	static void UnloadAllItems(UObject* Context, EConfirmPolicy& ConfirmPolicy, bool bForceUnload = false);
	
	UFUNCTION(BlueprintCallable, meta=(Categories="Inventory.Categories"), Category=Functionality)
	static TArray<UInventoryItemFragment*> GetItemsWithCategoryTag(const FGameplayTag& Tag);
	
	UFUNCTION(BlueprintCallable, Category=Trade, meta=(WorldContext="Context"))
	static void Trade(UWorld* Context, const TArray<FInventoryItem>& MoveItems, UInventoryComponent* From, UInventoryComponent* To);
};
