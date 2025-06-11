// © 2025 Beatriz Sabater Serna <dev@beatrizsabater.es>

#pragma once

#include "CoreMinimal.h"
#include "GA/GAOperators.h"
#include "GAMutationStrategy_UniqueGeneSwap.generated.h"

/**
 * 
 */
UCLASS()
class TFMPROJECT_API UGAMutationStrategy_UniqueGeneSwap : public UGAMutationStrategyAbstract
{
	GENERATED_BODY()

public:
	virtual void PerformMutation(UObject* WorldContextObject, TArray<FGAIndividual>& PopulationInOut, const TArray<int32>& CardPool, TArray<FGAMutationReportEntry>& OutReportEntries) override;
};
