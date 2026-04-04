// Copyright © 2026 bo6eR. All rights reserved.

#include "NetworkInventorySystem.h"
#include "System/Debug/InventoryLogContainer.h"

#define LOCTEXT_NAMESPACE "FNetworkInventorySystemModule"

void FNetworkInventorySystemModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	OnPostWorldCreateHandle = FWorldDelegates::OnPostWorldCreation.AddRaw(this, &FNetworkInventorySystemModule::BeginPlay);
	OnPostWorldCleanupHandle = FWorldDelegates::OnPostWorldCleanup.AddRaw(this, &FNetworkInventorySystemModule::BeginDestroy);
}

void FNetworkInventorySystemModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FWorldDelegates::OnPostWorldCreation.Remove(OnPostWorldCreateHandle);
	FWorldDelegates::OnPostWorldCleanup.Remove(OnPostWorldCleanupHandle);
}

void FNetworkInventorySystemModule::BeginPlay(UWorld* World)
{
	if (World != nullptr)
	{
		DebugTicker = MakeUnique<FInventoryDebugTicker>();
	}
}

void FNetworkInventorySystemModule::BeginDestroy(UWorld* World, bool bSessionEnded, bool bCleanupResources)
{
	DebugTicker.Reset();
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FNetworkInventorySystemModule, NetworkInventorySystem)