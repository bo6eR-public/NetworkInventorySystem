// Copyright © 2026 bo6eR. All rights reserved.


#include "System/Tests/FunctionalTest_AddInvItem.h"
#include "Components/InventoryComponent.h"
#include "System/InventoryUtilityTypes.h"
#include "FunctionalTestingModule.h"
#include "GameFramework/Character.h"
#include "EngineUtils.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FunctionalTest_AddInvItem)

#define FOREACH_CHARACTER(World)														\
	for (TActorIterator<ACharacter> It = TActorIterator<ACharacter>(World); It; ++It)	\
		if (ACharacter* Character = *It)											\

AFunctionalTest_AddInvItem::AFunctionalTest_AddInvItem()
{
	bAlwaysRelevant = true;
	bReplicates = true;
}

void AFunctionalTest_AddInvItem::PrepareTest()
{
	Super::PrepareTest();
	
	if (!Item.IsValid())
	{
		FinishTest(EFunctionalTestResult::Failed, TEXT("Failed to find CDO for InventoryItemFragment"));
	}
}

#if !UE_BUILD_SHIPPING

void AFunctionalTest_AddInvItem::StartTest()
{
	Super::StartTest();
	
	if (HasAuthority())
	{
		const UWorld* World = GetWorld();
		if (!World) return;
		
		FOREACH_CHARACTER(World)
		{
			if (UInventoryComponent* InventoryComponent = Character->FindComponentByClass<UInventoryComponent>())
			{
				InventoryComponent->OnInventoryUpdate.AddUniqueDynamic(this, &ThisClass::Client_OnInventoryUpdated);
        			
				const FInventoryInstance InventoryInstance(Item, Character);
				InventoryComponent->Server_AddItemAsync(InventoryInstance, true);
			}
		}
	}
}

void AFunctionalTest_AddInvItem::Client_OnInventoryUpdated_Implementation()
{
	const UWorld* World = GetWorld();
	if (!World) return;
	
	FOREACH_CHARACTER(World)
	{
		const UInventoryComponent* InventoryComponent = Character->FindComponentByClass<UInventoryComponent>();
		if (InventoryComponent && InventoryComponent->ContainsById(Item))
		{
			FinishTest(EFunctionalTestResult::Succeeded, TEXT("Success result for FunctionalTest_AddInvItem"));
		}
		else
		{
			FinishTest(EFunctionalTestResult::Failed, TEXT("Failed result for FunctionalTest_AddInvItem"));
		}
	}
}

#endif
