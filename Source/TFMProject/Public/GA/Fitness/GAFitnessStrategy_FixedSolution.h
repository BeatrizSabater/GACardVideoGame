// © 2025 Beatriz Sabater Serna <dev@beatrizsabater.es>

#pragma once

#include "CoreMinimal.h"
#include "GA/GAOperators.h"
#include "GAFitnessStrategy_FixedSolution.generated.h"

/**
 * 
 */
UCLASS()
class TFMPROJECT_API UGAFitnessStrategy_FixedSolution : public UGAFitnessStrategyAbstract
{
	GENERATED_BODY()

public:

	virtual float CalculateFitness(UObject* WorldContextObject, const FGAIndividual& Individual, const TArray<int32>& CardPool) override;
};
