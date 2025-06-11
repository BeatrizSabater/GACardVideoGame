// © 2025 Beatriz Sabater Serna <dev@beatrizsabater.es>

#pragma once

#include "CoreMinimal.h"
#include "Clash/ClashSubsystem.h"
#include "GA/GAOperators.h"
#include "GAFitnessStrategy_WRP_TurnsHealth.generated.h"

/**
 * 
 */
UCLASS()
class TFMPROJECT_API UGAFitnessStrategy_WRP_TurnsHealth : public UGAFitnessStrategyAbstract
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Strategy Settings")
	float WinRateWeight = 0.9f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Strategy Settings")
	int32 NumSimulationsPerIndividual = 5;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Strategy Settings")
	int32 OptimalTurnsPerformance = 2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Strategy Settings")
	float PerformanceTurnsWeight = 0.5f;

	virtual float CalculateFitness(UObject* WorldContextObject, const FGAIndividual& Individual, const TArray<int32>& CardPool) override;

private:
	
	FClashResult RunSimulation(UObject* WorldContextObject, const FGAIndividual& Individual, int32 ConsecutiveExecutionNumber);

	float CalculatePerformanceByClash(int32 PlayedTurns, FUnitStats PlayerUnitStats);
};
