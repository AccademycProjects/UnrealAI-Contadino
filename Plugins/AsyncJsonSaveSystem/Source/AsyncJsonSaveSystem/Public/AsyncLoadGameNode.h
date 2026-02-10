// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "PlayerSaveData.h"
#include "AsyncLoadGameNode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnAsyncLoadFinished,
	bool,
	bSuccess,
	FPlayerSaveData,
	LoadedData
);

UCLASS()
class ASYNCJSONSAVESYSTEM_API UAsyncLoadGameNode : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnAsyncLoadFinished Completed;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))
	static UAsyncLoadGameNode* LoadGameAsync(
		UObject* WorldContextObject,
		FString SlotName
	);

	virtual void Activate() override;

private:
	UPROPERTY()
	UObject* WorldContext;

	FString Slot;

	UFUNCTION()
	void HandleLoadCompleted(bool bSuccess, const FString& SlotName, FPlayerSaveData Data);
};
