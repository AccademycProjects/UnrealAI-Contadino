#include "EnvQueryTest_TagContainer.h"
#include "StateTagInterface.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_ActorBase.h"
#include "Engine/Engine.h"

UEnvQueryTest_TagContainer::UEnvQueryTest_TagContainer()
{
    Cost = EEnvTestCost::Low;
    ValidItemType = UEnvQueryItemType_ActorBase::StaticClass();
    TestPurpose = EEnvTestPurpose::FilterAndScore;
    SetWorkOnFloatValues(false);
}

void UEnvQueryTest_TagContainer::RunTest(FEnvQueryInstance& QueryInstance) const
{
#pragma region IterateItems

    for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
    {
        AActor* Actor = GetItemActor(QueryInstance, It.GetIndex());
        bool bMatch = false;

        FString DebugMessage = "";
        FColor DebugColor = FColor::Red;

        if (IsValid(Actor))
        {
            bool bImplements = Actor->GetClass()->ImplementsInterface(UStateTagInterface::StaticClass());

            if (bImplements)
            {
                FGameplayTagContainer ActorTags;
                
                IStateTagInterface::Execute_GetStateTags(Actor, ActorTags);

#pragma region MatchLogic

                switch (MatchType)
                {
                case EEQSTagMatchType::Any:
                    bMatch = ActorTags.HasAny(RequiredTags);
                    break;

                case EEQSTagMatchType::All:
                    bMatch = ActorTags.HasAll(RequiredTags);
                    break;

                case EEQSTagMatchType::Exact:
                    bMatch = (ActorTags == RequiredTags);
                    break;
                }

#pragma endregion
                
                if(It.GetIndex() < 5) 
                {
                    DebugMessage = FString::Printf(TEXT("Actor: %s | Tags Found: %s | Looking For: %s | MATCH: %s"), 
                        *Actor->GetName(), 
                        *ActorTags.ToString(), 
                        *RequiredTags.ToString(),
                        bMatch ? TEXT("TRUE") : TEXT("FALSE"));
                    
                    DebugColor = bMatch ? FColor::Green : FColor::Orange;
                }
            }
            else
            {
                 if(It.GetIndex() < 5)
                 {
                    DebugMessage = FString::Printf(TEXT("Actor: %s | INTERFACE MISSING in C++ check!"), *Actor->GetName());
                 }
            }

            if (bInvertResult)
            {
                bMatch = !bMatch;
            }
        }
        else
        {
            DebugMessage = TEXT("Actor Invalid or Null");
        }

        if (!DebugMessage.IsEmpty())
        {
            GEngine->AddOnScreenDebugMessage(-1, 0.0f, DebugColor, DebugMessage);
        }

#pragma region Output

        if (bScoreInsteadOfFilter)
        {
            const float Score = bMatch ? 1.0f : 0.0f;
            It.SetScore(TestPurpose, FilterType, Score, 0.0f, 1.0f);
        }
        else
        {
            It.SetScore(TestPurpose, FilterType, bMatch, true);
        }

#pragma endregion
    }

#pragma endregion
}