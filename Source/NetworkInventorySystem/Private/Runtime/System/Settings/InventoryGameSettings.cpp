// Copyright © 2026 bo6eR. All rights reserved.


#include "System/Settings/InventoryGameSettings.h"
#include "Fragments/InventoryCategoryFragment.h"
#include "System/Debug/InventoryLogContainer.h"
#include "Engine/AssetManager.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(InventoryGameSettings)

UInventoryGameSettings::UInventoryGameSettings(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Smaller sub-heading for grouping
	SectionName = "Inventory Categories";
}

void UInventoryGameSettings::PostInitProperties()
{
	Super::PostInitProperties();
	FWorldDelegates::OnPostWorldCreation.AddUObject(this, &ThisClass::BeginPlay);
}

const UInventoryGameSettings* UInventoryGameSettings::Get()
{
	return GetDefault<UInventoryGameSettings>();
}

const UInventoryCategoryFragment* UInventoryGameSettings::GetCategoryByTag(const FGameplayTag& Tag) const
{
	/* If we've found one loaded category, so we have already whole loaded array */
	if (Categories.FindByPredicate([](TSoftObjectPtr<UInventoryCategoryFragment> Entry){ return Entry.IsValid(); } ))
	{
		for (const auto& Entry : Categories)
		{
			if (Entry->CategoryTag.IsValid() && Entry->CategoryTag == Tag)
			{
				return Entry.Get();
			}
		}
	}
	else
	{
		for (const auto& Entry : RequestCategoriesSyncLoad())
		{
			if (Entry->CategoryTag.IsValid() && Entry->CategoryTag == Tag)
			{
				return Entry;
			}
		}
	}
	return nullptr;
}

FName UInventoryGameSettings::GetCategoryName() const
{
	// Large text for where settings should be grouped on the left
	return TEXT("Inventory System");
}

#if WITH_EDITOR
FText UInventoryGameSettings::GetSectionDescription() const
{
	return NSLOCTEXT("InventoryGameSettingsAdvanced", "InventoryCategoriesDescription", "Everything related to categories for the inventory system.");
}
#endif

TArray<UInventoryCategoryFragment*> UInventoryGameSettings::RequestCategoriesSyncLoad() const
{
	const UAssetManager* AssetManager =  UAssetManager::GetIfInitialized();
	check(AssetManager);
	
	TArray<UInventoryCategoryFragment*> OutFragments;
	
	if (!Categories.IsEmpty())
	{
		/* If we've found one loaded category, so we have already whole loaded array */
		if (Categories.FindByPredicate([](TSoftObjectPtr<UInventoryCategoryFragment> Entry){ return Entry.IsValid(); } ))
		{
			for (const auto Entry : Categories)
			{
				OutFragments.Add(Entry.Get());
			}
			return OutFragments;
		}
	}
	
	for (const auto Entry : Categories)
	{
		if (const auto CategoryFragment = AssetManager->GetStreamableManager().LoadSynchronous(Entry))
		{
			OutFragments.Add(CategoryFragment);
		}
	}
	return OutFragments;
}

void UInventoryGameSettings::BeginPlay(UWorld* World)
{
	if (World != nullptr)
	{
		if (bIsCategorized)
		{
			TArray Cats = RequestCategoriesSyncLoad();
		}
	}
}

#if WITH_EDITOR
void UInventoryGameSettings::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	
	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(ThisClass, bUseInternalLogs))
	{
		LogInventory.SetVerbosity(bUseInternalLogs ? ELogVerbosity::Log : ELogVerbosity::NoLogging);
	}
}
#endif
