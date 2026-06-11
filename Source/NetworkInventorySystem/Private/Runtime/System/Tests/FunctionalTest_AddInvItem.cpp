// Copyright © 2026 bo6eR. All rights reserved.


#include "System/Tests/FunctionalTest_AddInvItem.h"
#include "Components/InventoryComponent.h"
#include "System/InventoryUtilityTypes.h"
#include "FunctionalTestingModule.h"
#include "GameFramework/Character.h"
#include "EngineUtils.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FunctionalTest_AddInvItem)

AFunctionalTest_AddInvItem::AFunctionalTest_AddInvItem()
{
	// Does something...
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
        	
        for (TActorIterator<ACharacter> It = TActorIterator<ACharacter>(World); It; ++It)
        {
        	if (ACharacter* Character = *It)
        	{
		        if (UInventoryComponent* InventoryComponent = Character->FindComponentByClass<UInventoryComponent>())
        		{
        			InventoryComponent->OnInventoryUpdate.AddUniqueDynamic(this, &ThisClass::OnInventoryUpdated);
        			
        			const FInventoryInstance InventoryInstance(Item, Character);
        			InventoryComponent->Server_AddItemAsync(InventoryInstance, true);
        		}
        	}
        }
	}
}

void AFunctionalTest_AddInvItem::OnInventoryUpdated()
{
	const UWorld* World = GetWorld();
	if (!World) return;
        	
	for (TActorIterator<ACharacter> It = TActorIterator<ACharacter>(World); It; ++It)
	{
		if (const ACharacter* Character = *It)
		{
			const UInventoryComponent* InventoryComponent = Character->FindComponentByClass<UInventoryComponent>();
			if (InventoryComponent && InventoryComponent->ContainsById(Item))
			{
				FinishTest(EFunctionalTestResult::Succeeded, TEXT("Success result for FunctionalTest_AddInvItem"));
			}
			else if (InventoryComponent && !InventoryComponent->ContainsById(Item))
			{
				FinishTest(EFunctionalTestResult::Failed, TEXT("Failed result for FunctionalTest_AddInvItem"));
			}
		}
	}
}

#endif
