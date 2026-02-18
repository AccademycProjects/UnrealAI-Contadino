// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayTagContainer.h"
#include "StateTagInterface.generated.h"

UINTERFACE(BlueprintType)
class UIUNREALTEST_API UStateTagInterface : public UInterface
{
	GENERATED_BODY()
};

class UIUNREALTEST_API IStateTagInterface
{
	GENERATED_BODY()

public:

	// Must fill the container with all relevant state tags
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="State")
	void GetStateTags(FGameplayTagContainer& OutTags) const;
};
