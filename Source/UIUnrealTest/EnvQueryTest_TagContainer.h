// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTest.h"
#include "GameplayTagContainer.h"
#include "EQSTagMatchType.h"
#include "EnvQueryTest_TagContainer.generated.h"

UCLASS()
class UIUNREALTEST_API UEnvQueryTest_TagContainer : public UEnvQueryTest
{
	GENERATED_BODY()

public:
	UEnvQueryTest_TagContainer();

protected:

#pragma region Config

	// Tags required for the test
	UPROPERTY(EditDefaultsOnly, Category="Tags")
	FGameplayTagContainer RequiredTags;

	// Defines how RequiredTags are evaluated
	UPROPERTY(EditDefaultsOnly, Category="Tags")
	EEQSTagMatchType MatchType = EEQSTagMatchType::Any;

	// Inverts final result
	UPROPERTY(EditDefaultsOnly, Category="Tags")
	bool bInvertResult = false;

	// If true, will score instead of pure filter
	UPROPERTY(EditDefaultsOnly, Category="Scoring")
	bool bScoreInsteadOfFilter = false;

#pragma endregion

	virtual void RunTest(FEnvQueryInstance& QueryInstance) const override;
};
