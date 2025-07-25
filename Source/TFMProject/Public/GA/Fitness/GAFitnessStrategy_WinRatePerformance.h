// © 2025 Beatriz Sabater Serna <dev@beatrizsabater.es>

#pragma once

#include "CoreMinimal.h"
#include "Clash/ClashSubsystem.h"
#include "GA/GAOperators.h"
#include "GAFitnessStrategy_WinRatePerformance.generated.h"

/**
 * 
 */
UCLASS()
class TFMPROJECT_API UGAFitnessStrategy_WinRatePerformance : public UGAFitnessStrategyAbstract
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Strategy Settings")
	float WinRateWeight = 0.9f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Strategy Settings")
	float PerformanceWeight = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Strategy Settings")
	int32 NumSimulationsPerIndividual = 5;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Strategy Settings")
	int32 OptimalTurnsPerformance = 2;

	virtual float CalculateFitness(UObject* WorldContextObject, const FGAIndividual& Individual, const TArray<int32>& CardPool) override;

private:
	
	FClashResult RunSimulation(UObject* WorldContextObject, const FGAIndividual& Individual);
};
