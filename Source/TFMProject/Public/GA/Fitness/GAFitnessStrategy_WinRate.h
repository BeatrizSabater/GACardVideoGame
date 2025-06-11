// © 2025 Beatriz Sabater Serna <dev@beatrizsabater.es>

#pragma once

#include "CoreMinimal.h"
#include "Clash/ClashSubsystem.h"
#include "GA/GAOperators.h"
#include "GAFitnessStrategy_WinRate.generated.h"

class UGAConfigDataAsset;
/**
 * 
 */
UCLASS()
class TFMPROJECT_API UGAFitnessStrategy_WinRate : public UGAFitnessStrategyAbstract
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Strategy Settings")
	int32 NumSimulationsPerIndividual = 5;

	virtual float CalculateFitness(UObject* WorldContextObject, const FGAIndividual& Individual, const TArray<int32>& CardPool) override;

private:
	
	FClashResult RunSimulation(UObject* WorldContextObject, const FGAIndividual& Individual);
};
