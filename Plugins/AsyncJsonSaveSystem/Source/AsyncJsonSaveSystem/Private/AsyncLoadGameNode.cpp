// Fill out your copyright notice in the Description page of Project Settings.

#include "AsyncLoadGameNode.h"
#include "AsyncJsonSaveSubsystem.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"

UAsyncLoadGameNode* UAsyncLoadGameNode::LoadGameAsync(
	UObject* WorldContextObject,
	FString SlotName)
{
	UAsyncLoadGameNode* Node = NewObject<UAsyncLoadGameNode>();
	Node->WorldContext = WorldContextObject;
	Node->Slot = SlotName;
	return Node;
}

void UAsyncLoadGameNode::Activate()
{
	if (!WorldContext)
	{
		Completed.Broadcast(false, FPlayerSaveData());
		SetReadyToDestroy();
		return;
	}

	UWorld* World = WorldContext->GetWorld();
	if (!World)
	{
		Completed.Broadcast(false, FPlayerSaveData());
		SetReadyToDestroy();
		return;
	}

	UGameInstance* GI = World->GetGameInstance();
	if (!GI)
	{
		Completed.Broadcast(false, FPlayerSaveData());
		SetReadyToDestroy();
		return;
	}

	UAsyncJsonSaveSubsystem* Subsystem =
		GI->GetSubsystem<UAsyncJsonSaveSubsystem>();

	if (!Subsystem)
	{
		Completed.Broadcast(false, FPlayerSaveData());
		SetReadyToDestroy();
		return;
	}

	// Ensure no duplicate bindings
	Subsystem->OnLoadCompleted.RemoveAll(this);

	// Bind subsystem delegate to this async node
	Subsystem->OnLoadCompleted.AddDynamic(
		this,
		&UAsyncLoadGameNode::HandleLoadCompleted
	);

	Subsystem->LoadGameAsync(Slot);
}

void UAsyncLoadGameNode::HandleLoadCompleted(
	bool bSuccess,
	const FString& SlotName,
	FPlayerSaveData Data)
{
	Completed.Broadcast(bSuccess, Data);
	SetReadyToDestroy();
}
