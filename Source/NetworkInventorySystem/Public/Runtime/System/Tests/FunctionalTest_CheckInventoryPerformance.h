// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FunctionalTest.h"
#include "FunctionalTest_CheckInventoryPerformance.generated.h"

class UInventoryItemFragment;

UCLASS()
class NETWORKINVENTORYSYSTEM_API AFunctionalTest_CheckInventoryPerformance : public AFunctionalTest
{
	GENERATED_BODY()

public:
	AFunctionalTest_CheckInventoryPerformance();

protected:
	//~ Begin of AFunctionalTest Interface
	virtual void PrepareTest() override;
	virtual void StartTest() override;
	//~ End of AFunctionalTest Interface
	
private:
	UFUNCTION()
	void OnInventoryUpdated();
	
private:
	UPROPERTY()
	TObjectPtr<UInventoryItemFragment> ItemCDO = nullptr;
};
