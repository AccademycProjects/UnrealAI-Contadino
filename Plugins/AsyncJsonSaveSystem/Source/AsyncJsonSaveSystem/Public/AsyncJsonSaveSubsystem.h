// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "PlayerSaveData.h"
#include "AsyncJsonSaveSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSaveCompleted, bool, bSuccess, const FString&, SlotName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnLoadCompleted, bool, bSuccess, const FString&, SlotName, FPlayerSaveData, Data);

UCLASS(BlueprintType)
class ASYNCJSONSAVESYSTEM_API UAsyncJsonSaveSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnSaveCompleted OnSaveCompleted;

	UPROPERTY(BlueprintAssignable)
	FOnLoadCompleted OnLoadCompleted;

	UFUNCTION(BlueprintCallable)
	void SaveGameAsync(const FPlayerSaveData& Data, const FString& SlotName);

	UFUNCTION(BlueprintCallable)
	void LoadGameAsync(const FString& SlotName);

private:
	FString GetSaveDir() const;
	FString GetFullPathForSlot(const FString& SlotName) const;
};
