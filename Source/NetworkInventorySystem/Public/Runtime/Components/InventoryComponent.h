// Copyright © 2026 bo6eR. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Runtime/InventoryCoreTypes.h"
#include "InventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInventoryUpdate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInventoryEmpty);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPreReplicatedRemoveDelegate, const FInventoryArray&, InArray);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPostReplicatedAddDelegate, const FInventoryArray&, InArray);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPostReplicatedChangeDelegate, const FInventoryArray&, InArray);

//~ Forwarding declaration
struct FStreamableHandle;
struct FInventoryInstance;
class UInventoryItemFragment;
enum class EInventorySortOrder : uint8;
enum class EMatchType : uint8;
//~ End of forwarding declaration

UCLASS(ClassGroup="Inventory System", meta=(BlueprintSpawnableComponent), DisplayName="Inventory Component", Blueprintable, BlueprintType)
class NETWORKINVENTORYSYSTEM_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	explicit UInventoryComponent(const FObjectInitializer& ObjectInitializer);
	
#pragma region Getters
	UFUNCTION(BlueprintCallable, BlueprintPure, meta=(CompactNodeTitle="GET ALL"), Category=Functionality)
	TArray<FInventoryItem> GetItems() const;
	
	UFUNCTION(BlueprintPure, meta=(CompactNodeTitle="GET REF"), Category=Functionality)
	FInventoryItem& GetItemRefAt(const int32 Index);
	
	UFUNCTION(BlueprintPure, meta=(CompactNodeTitle="GET COPY"), Category=Functionality)
	FInventoryItem GetItemCopyAt(const int32 Index);
#pragma endregion
	
	UFUNCTION(BlueprintCallable, Server, Reliable, DisplayName="Add Item Async", Category=Functionality)
	void Server_AddItemAsync(const FInventoryInstance& InventoryDataInfo, bool bForceLoad = false);
	
	UFUNCTION(BlueprintCallable, Server, Reliable, DisplayName="Delete Item Async", Category=Functionality)
	void Server_DeleteItemAsync(const FInventoryInstance& InventoryDataInfo);
	
	UFUNCTION(BlueprintCallable, Server, Reliable, DisplayName="Unload All Items And Clear Inventory", Category=Functionality)
	void Server_ClearInventory();
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category=Functionality)
	bool Contains(const UInventoryItemFragment* EntryFragment) const;
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category=Functionality)
	const FInventoryItem& FindBy(const UInventoryItemFragment* EntryFragment) const;
	
	UFUNCTION(BlueprintCallable, CustomThunk, BlueprintPure=false, DisplayName="Sort Items", Category=Functionality)
	void BP_SortItems(TArray<FInventoryItem>& Sorted, EInventorySortOrder SortOrder, const FName ParamName) const;
	
	UFUNCTION(BlueprintCallable, CustomThunk, BlueprintPure=false, DisplayName="Filter Items", Category=Functionality)
	void BP_FilterItems(TArray<FInventoryItem>& Filtered, FGameplayTagContainer CategoriesTagContainer, EMatchType MatchType) const;
	
	template<typename Predicate>
	void Sort_Internal(TArray<FInventoryItem>& Sorted, Predicate Function);
	template<typename Predicate>
	void Filter_Internal(TArray<FInventoryItem>& Filtered, Predicate Function);

#pragma region Debug
	UFUNCTION(Exec, BlueprintCallable, Category=Debug, BlueprintPure=false, meta=(DevelopmentOnly))
	static void PrintInventoryItems(const TArray<FInventoryItem>& Items, float TimeToDisplay, FLinearColor Color);
#pragma endregion
	
protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	virtual void ReadyForReplication() override;
	
	UFUNCTION(BlueprintNativeEvent)
	void OnRep_InventoryArray();
	
private:
	DECLARE_FUNCTION(execBP_SortItems);
	DECLARE_FUNCTION(execBP_FilterItems);
	
public:
	UPROPERTY(BlueprintAssignable, Category="Server Inventory Events")
	FInventoryUpdate OnInventoryUpdate;
	
	UPROPERTY(BlueprintAssignable, Category="Server Inventory Events")
	FInventoryEmpty OnInventoryEmpty;
	
#pragma region OnRep Delegates
	UPROPERTY(BlueprintAssignable, Category="Client Inventory Events")
	FPreReplicatedRemoveDelegate PreReplicatedRemoveDelegate;
	
	UPROPERTY(BlueprintAssignable, Category="Client Inventory Events")
	FPostReplicatedAddDelegate PostReplicatedAddDelegate;
	
	UPROPERTY(BlueprintAssignable, Category="Client Inventory Events")
	FPostReplicatedChangeDelegate PostReplicatedChangeDelegate;
#pragma endregion

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Size, meta=(AllowPrivateAccess=true, ClampMin="0"))
	/* The number is used for reserve operation of inventory items array */
	int32 MaxInventorySize = 256;
	
	UPROPERTY(ReplicatedUsing=OnRep_InventoryArray)
	FInventoryArray InventoryArray;
	
	/* Cached handle for loading the primary asset ids */
	TSharedPtr<FStreamableHandle> PostLoadHandle;
};
