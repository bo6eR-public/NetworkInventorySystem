// Copyright © 2026 bo6eR. All rights reserved.

#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(LogInventory, NoLogging, All);

/*
 * Debug container for inventory logging. It provides tick function that calls the printing of inventory array
 */

static TAutoConsoleVariable<int32> CVarInventoryDebug
(
	TEXT("InventorySystem.Debug.Show"),
	0,
	TEXT("Provides the show of inventory items on the client")
);

class FInventoryDebugTicker
{
public:
	FInventoryDebugTicker()
	{
		UE_LOG(LogInventory, Log, TEXT("FInventoryDebugTicker"));
		TickerHandle = FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateRaw(this, &FInventoryDebugTicker::Tick));
	}

	virtual ~FInventoryDebugTicker()
	{
		UE_LOG(LogInventory, Log, TEXT("~FInventoryDebugTicker"));
		FTSTicker::GetCoreTicker().RemoveTicker(TickerHandle);
	}
	
protected:
	virtual bool Tick(float DeltaTime);
	
private:
	static void Print();
	
private:
	FTSTicker::FDelegateHandle TickerHandle;
};
