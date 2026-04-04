// // Copyright © 2026 bo6eR. All rights reserved.

#pragma once

#include "Engine/DeveloperSettings.h"
#include "InventoryGameSettings.generated.h"

//~ Forwarding declaration
struct FGameplayTag;
class UInventoryCategoryFragment;
//~ End of forwarding declaration

/*
 * Inventory settings gives such technical information
 */

UCLASS(Config=Game, DefaultConfig)
class NETWORKINVENTORYSYSTEM_API UInventoryGameSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	explicit UInventoryGameSettings(const FObjectInitializer& ObjectInitializer);
	virtual void PostInitProperties() override;
	
	static const UInventoryGameSettings* Get();
	
	UFUNCTION()
	const UInventoryCategoryFragment* GetCategoryByTag(const FGameplayTag& Tag) const;
	UFUNCTION()
	FORCEINLINE bool IsCategorized() const { return bIsCategorized; }
	
	//~ Begin UDeveloperSettings interface
	virtual FName GetCategoryName() const override;
#if WITH_EDITOR
	virtual FText GetSectionDescription() const override;
#endif
	//~ End UDeveloperSettings interface
	
	UFUNCTION(BlueprintCallable, BlueprintPure=false, DisplayName="Get Inventory Categories", Category=Functionality)
	/* Returns the categories array or load sync the soft categories array and return this one */
	TArray<UInventoryCategoryFragment*> RequestCategoriesSyncLoad() const;
	
protected:
	UFUNCTION()
	virtual void BeginPlay(UWorld* World);
	
private:
#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

private:
#if WITH_EDITORONLY_DATA
	UPROPERTY(Config, EditAnywhere, Category=Default)
	/* Should the plugin use own log system */
	bool bUseInternalLogs = false;
#endif
	
	UPROPERTY(Config, EditAnywhere, Category=Default)
	/* The categorization could be used for filtering, but optionally the inventory system uses non-ordered structure */
	bool bIsCategorized = true;
	
	UPROPERTY(Config, EditAnywhere, meta=(EditCondition=bIsCategorized), Category=Default)
	/* The categories for inventory items. They are based asset type on */
	TArray<TSoftObjectPtr<UInventoryCategoryFragment>> Categories;
};
