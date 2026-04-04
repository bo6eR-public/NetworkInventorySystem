// Copyright © 2026 bo6eR. All rights reserved.


#include "System/Debug/InventoryLogContainer.h"
#include "Components/InventoryComponent.h"

DEFINE_LOG_CATEGORY(LogInventory);

bool FInventoryDebugTicker::Tick(float DeltaTime)
{
	if (CVarInventoryDebug.GetValueOnAnyThread())
	{
		AsyncTask(ENamedThreads::Type::GameThread, []{ Print(); });
		return true;
	}
	return false;
}

void FInventoryDebugTicker::Print()
{
	const UWorld* World = GEngine->GameViewport->GetWorld();
	check(World);
	
	if (const APlayerController* PlayerController = World->GetFirstPlayerController())
	{
		if (const APawn* Pawn = PlayerController->GetPawn())
		{
			if (UInventoryComponent* InventoryComponent = Cast<UInventoryComponent>(Pawn->FindComponentByClass(UInventoryComponent::StaticClass())))
			{
				/* Client getter print function */
				InventoryComponent->PrintInventoryItems(InventoryComponent->GetItems(), 0.f, FLinearColor::Green);
			}
		}
	}
};
