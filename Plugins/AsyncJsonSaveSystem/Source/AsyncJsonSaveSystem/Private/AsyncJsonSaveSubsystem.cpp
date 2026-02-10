// Fill out your copyright notice in the Description page of Project Settings.

#include "AsyncJsonSaveSubsystem.h"

#include "JsonObjectConverter.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "HAL/PlatformFilemanager.h"
#include "Async/Async.h"

#pragma region Paths & Utilities

FString UAsyncJsonSaveSubsystem::GetSaveDir() const
{
    return FPaths::ProjectSavedDir() / TEXT("SaveGames/");
}

FString UAsyncJsonSaveSubsystem::GetFullPathForSlot(const FString& SlotName) const
{
    return GetSaveDir() / (SlotName + TEXT(".json"));
}

static FString SanitizeSlotName(const FString& InSlotName)
{
    FString CleanName = InSlotName;
    CleanName.ReplaceInline(TEXT("/"), TEXT(""));
    CleanName.ReplaceInline(TEXT("\\"), TEXT(""));
    CleanName.ReplaceInline(TEXT(":"), TEXT(""));
    CleanName.ReplaceInline(TEXT(" "), TEXT("_"));

    return CleanName.IsEmpty() ? TEXT("DefaultSlot") : CleanName;
}

#pragma endregion

#pragma region Save

void UAsyncJsonSaveSubsystem::SaveGameAsync(const FPlayerSaveData& Data, const FString& SlotName)
{
    // Serialize on GameThread (safe and fast)
    FString JsonString;
    if (!FJsonObjectConverter::UStructToJsonObjectString(Data, JsonString))
    {
        OnSaveCompleted.Broadcast(false, SlotName);
        return;
    }

    const FString CleanSlotName = SanitizeSlotName(SlotName);
    const FString SaveDir = GetSaveDir();
    const FString FinalPath = GetFullPathForSlot(CleanSlotName);
    const FString TempPath = FinalPath + TEXT(".tmp");

    // Heavy I/O work on background thread
    Async(EAsyncExecution::ThreadPool, [this, JsonString, SaveDir, TempPath, FinalPath, CleanSlotName]()
    {
        IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
        PlatformFile.CreateDirectoryTree(*SaveDir);

        // ONLY FOR TESTING: simulate slow disk I/O
        FPlatformProcess::Sleep(2.0f);

        bool bSaved = FFileHelper::SaveStringToFile(JsonString, *TempPath);

        // Atomic replace to avoid corrupted save files
        if (bSaved)
        {
            PlatformFile.DeleteFile(*FinalPath);
            bSaved = PlatformFile.MoveFile(*FinalPath, *TempPath);
        }

        // Notify Blueprint on GameThread
        AsyncTask(ENamedThreads::GameThread, [this, bSaved, CleanSlotName]()
        {
            OnSaveCompleted.Broadcast(bSaved, CleanSlotName);
        });
    });
}

#pragma endregion

#pragma region Load

void UAsyncJsonSaveSubsystem::LoadGameAsync(const FString& SlotName)
{
    const FString CleanSlotName = SanitizeSlotName(SlotName);
    const FString FullPath = GetFullPathForSlot(CleanSlotName);

    // Load + parse entirely on background thread
    Async(EAsyncExecution::ThreadPool, [this, CleanSlotName, FullPath]()
    {
        FString JsonString;
        FPlayerSaveData LoadedData;
        bool bSuccess = false;

        if (FFileHelper::LoadFileToString(JsonString, *FullPath))
        {
            bSuccess = FJsonObjectConverter::JsonObjectStringToUStruct(
                JsonString,
                &LoadedData,
                0,
                0
            );
        }

        // Return result on GameThread
        AsyncTask(ENamedThreads::GameThread, [this, bSuccess, CleanSlotName, LoadedData]()
        {
            OnLoadCompleted.Broadcast(bSuccess, CleanSlotName, LoadedData);
        });
    });
}

#pragma endregion
