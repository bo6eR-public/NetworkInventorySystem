// Copyright © 2026 bo6eR. All rights reserved.

#pragma once

#include "Modules/ModuleManager.h"

//~ Forwarding declaration
class FInventoryDebugTicker;
//~ End of forwarding declaration

class FNetworkInventorySystemModule : public IModuleInterface
{
public:
	/* IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
protected:
	void BeginPlay(UWorld* World);
	void BeginDestroy(UWorld* World, bool bSessionEnded, bool bCleanupResources);
	
private:
	TUniquePtr<FInventoryDebugTicker> DebugTicker;
	
	FDelegateHandle OnPostWorldCreateHandle;
	FDelegateHandle OnPostWorldCleanupHandle;
};
