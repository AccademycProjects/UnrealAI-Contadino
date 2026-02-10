// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "PlayerSaveData.h"
#include "AsyncSaveGameNode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAsyncSaveFinished, bool, bSuccess);

UCLASS()
class ASYNCJSONSAVESYSTEM_API UAsyncSaveGameNode : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnAsyncSaveFinished Completed;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))
	static UAsyncSaveGameNode* SaveGameAsync(
		UObject* WorldContextObject,
		FPlayerSaveData Data,
		FString SlotName
	);
	
	UFUNCTION()
	void HandleSaveCompleted(bool bSuccess, const FString& SlotName);

	virtual void Activate() override;

private:
	UPROPERTY()
	UObject* WorldContext;

	FPlayerSaveData SaveData;
	FString Slot;
};
