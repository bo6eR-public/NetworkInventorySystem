// Copyright © 2026 bo6eR. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "FunctionalTest.h"
#include "FunctionalTest_AddInvItem.generated.h"

class UInventoryItemFragment;

UCLASS()
class NETWORKINVENTORYSYSTEM_API AFunctionalTest_AddInvItem : public AFunctionalTest
{
	GENERATED_BODY()

public:
	AFunctionalTest_AddInvItem();
	
#if !UE_BUILD_SHIPPING

protected:
	//~ Begin of AFunctionalTest Interface
	virtual void PrepareTest() override;
	virtual void StartTest() override;
	//~ End of AFunctionalTest Interface
	
#endif
	
private:
	UFUNCTION(Client, Unreliable)
	void Client_OnInventoryUpdated();
	
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item Data", meta=(AllowPrivateAccess=true))
	FPrimaryAssetId Item;
};


