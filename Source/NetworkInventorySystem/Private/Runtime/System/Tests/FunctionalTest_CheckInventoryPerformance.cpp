// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Tests/FunctionalTest_CheckInventoryPerformance.h"
#include "Components/InventoryComponent.h"
#include "System/InventoryUtilityTypes.h"
#include "FunctionalTestingModule.h"
#include "GameFramework/Character.h"
#include "EngineUtils.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FunctionalTest_CheckInventoryPerformance)

AFunctionalTest_CheckInventoryPerformance::AFunctionalTest_CheckInventoryPerformance()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AFunctionalTest_CheckInventoryPerformance::PrepareTest()
{
	Super::PrepareTest();
	
	ItemCDO = const_cast<UInventoryItemFragment*>(GetDefault<UInventoryItemFragment>());
	checkf(!ItemCDO, TEXT("Couldn't create CDO for Inventory Item Fragment"));
}

void AFunctionalTest_CheckInventoryPerformance::StartTest()
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
        			if (!ItemCDO) continue;
        			
        			InventoryComponent->OnInventoryUpdate.AddUniqueDynamic(this, &ThisClass::OnInventoryUpdated);
        			
        			const FInventoryInstance InventoryInstance(ItemCDO->GetPrimaryAssetId(), Character);
        			InventoryComponent->Server_AddItemAsync(InventoryInstance, true);
        		}
        	}
        }
	}
}

void AFunctionalTest_CheckInventoryPerformance::OnInventoryUpdated()
{
	const UWorld* World = GetWorld();
	if (!World) return;
        	
	for (TActorIterator<ACharacter> It = TActorIterator<ACharacter>(World); It; ++It)
	{
		if (const ACharacter* Character = *It)
		{
			FString LogMessage;
			const UInventoryComponent* InventoryComponent = Character->FindComponentByClass<UInventoryComponent>();
			if (InventoryComponent && InventoryComponent->Contains(ItemCDO))
			{
				FinishTest(EFunctionalTestResult::Succeeded, LogMessage);
			}
			else if (InventoryComponent && !InventoryComponent->Contains(ItemCDO))
			{
				FinishTest(EFunctionalTestResult::Failed, LogMessage);
			}
			UE_LOG(LogFunctionalTest, Log, TEXT("%s"), *LogMessage);
		}
	}
}
