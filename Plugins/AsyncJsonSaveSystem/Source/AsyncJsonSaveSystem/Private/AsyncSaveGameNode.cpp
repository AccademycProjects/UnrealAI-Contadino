// Fill out your copyright notice in the Description page of Project Settings.

#include "AsyncSaveGameNode.h"
#include "AsyncJsonSaveSubsystem.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"

UAsyncSaveGameNode* UAsyncSaveGameNode::SaveGameAsync(
	UObject* WorldContextObject,
	FPlayerSaveData Data,
	FString SlotName)
{
	UAsyncSaveGameNode* Node = NewObject<UAsyncSaveGameNode>();
	Node->WorldContext = WorldContextObject;
	Node->SaveData = Data;
	Node->Slot = SlotName;
	return Node;
}

void UAsyncSaveGameNode::Activate()
{
	if (!WorldContext)
	{
		Completed.Broadcast(false);
		SetReadyToDestroy();
		return;
	}

	UWorld* World = WorldContext->GetWorld();
	if (!World)
	{
		Completed.Broadcast(false);
		SetReadyToDestroy();
		return;
	}

	UGameInstance* GI = World->GetGameInstance();
	UAsyncJsonSaveSubsystem* Subsystem = GI->GetSubsystem<UAsyncJsonSaveSubsystem>();

	if (!Subsystem)
	{
		Completed.Broadcast(false);
		SetReadyToDestroy();
		return;
	}

	// Ensure no duplicate bindings
	Subsystem->OnSaveCompleted.RemoveAll(this);
	
	// Bind to subsystem delegate
	Subsystem->OnSaveCompleted.AddDynamic(
		this,
		&UAsyncSaveGameNode::HandleSaveCompleted
	);

	Subsystem->SaveGameAsync(SaveData, Slot);
}

void UAsyncSaveGameNode::HandleSaveCompleted(bool bSuccess, const FString& SlotName)
{
	Completed.Broadcast(bSuccess);
	SetReadyToDestroy();
}
