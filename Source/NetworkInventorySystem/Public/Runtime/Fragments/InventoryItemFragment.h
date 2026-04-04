// Copyright © 2026 bo6eR. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "InventoryItemFragment.generated.h"

//~ Forwarding declaration
class UInventoryCategoryFragment;
//~ End of forwarding declaration

namespace ItemAssetTypes
{
	static FPrimaryAssetType Item = FPrimaryAssetType(TEXT("Item"));
}

UCLASS(Abstract, BlueprintType)
class NETWORKINVENTORYSYSTEM_API UInventoryItemFragment : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category=Functionality)
	virtual void Use() PURE_VIRTUAL();
	
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category=Functionality)
	bool IsCategorized() const;
	
#pragma region Setters
    UFUNCTION(BlueprintCallable, BlueprintSetter)
    FORCEINLINE void SetCount(int32 NewCount) { Count = FMath::Max(0, NewCount); }
#pragma endregion
	
#pragma region Getters
	UFUNCTION(BlueprintPure, BlueprintGetter)
	FORCEINLINE FGameplayTag GetCategoryTag() const { return IsCategorized() ? CategoryTag : FGameplayTag::EmptyTag; }
	
	UFUNCTION(BlueprintPure, Category=Functionality)
	const UInventoryCategoryFragment* GetCategoryByTag() const;
	
	UFUNCTION(BlueprintPure, BlueprintGetter)
	FORCEINLINE int32 GetCount() const { return Count; }
	
	UFUNCTION(BlueprintPure, BlueprintGetter)
	FORCEINLINE FName GetItemName() const { return Name; }
	
	UFUNCTION(BlueprintPure, BlueprintGetter)
	FORCEINLINE FString GetDescription() const { return Description; }
	
	UFUNCTION(BlueprintPure, BlueprintGetter)
	FORCEINLINE bool GetIsStackable() const { return bIsStackable; }
	
	UFUNCTION(BlueprintPure, BlueprintGetter)
	FORCEINLINE UTexture2D* GetTexture() const { return Texture.Get(); }
	
	UFUNCTION(BlueprintPure, BlueprintGetter)
	FORCEINLINE USkeletalMesh* GetMesh() const { return Mesh.Get(); }
#pragma endregion
	
	UFUNCTION(BlueprintNativeEvent)
	void OnRep_Count();
	
	UFUNCTION(BlueprintNativeEvent)
	void OnRep_Texture();
	
	UFUNCTION(BlueprintNativeEvent)
	void OnRep_Mesh();
	
protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Mainline", meta=(Categories="Inventory.Categories", EditCondition="IsCategorized()", EditConditionHides=true))
	FGameplayTag CategoryTag;
	
	UPROPERTY(BlueprintReadOnly, Category="Primary Asset Data")
	FPrimaryAssetType AssetType = ItemAssetTypes::Item;
	
private:
	friend class UInventoryComponent;
	
	__readonly TSharedPtr<FGuid> Id = MakeShared<FGuid>(FGuid::NewGuid());
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess=true), Category="Information")
	FName Name = FName(TEXT("Unknown"));
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess=true), Category="Information")
	FString Description = FString(TEXT("Something"));
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess=true), Category="Advanced")
	bool bIsStackable = false;
	
	UPROPERTY(ReplicatedUsing=OnRep_Count, BlueprintReadOnly, meta=(AllowPrivateAccess=true, EditCondition=bStackable), Category="Advanced")
	int32 Count = 1;

#pragma region Bundles
	UPROPERTY(ReplicatedUsing=OnRep_Texture, EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess=true, AssetBundles="Texture"), Category="Apperance")
	TSoftObjectPtr<UTexture2D> Texture;
	
	UPROPERTY(ReplicatedUsing=OnRep_Mesh, EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess=true, AssetBundles="Mesh"), Category="Apperance")
	TSoftObjectPtr<USkeletalMesh> Mesh;
#pragma endregion
};
