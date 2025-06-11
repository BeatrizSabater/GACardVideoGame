// © 2025 Beatriz Sabater Serna <dev@beatrizsabater.es>

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GAConfigDataAsset.generated.h"


class UGASelectionStrategyAbstract;
class UGACrossoverStrategyAbstract;
class UGAMutationStrategyAbstract;
class UGAFitnessStrategyAbstract;

UCLASS(Blueprintable, BlueprintType)
class TFMPROJECT_API UGAConfigDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	// --- General genetic algorithm configuration parameters ---
	/**
	 * [INSERT POPULATION SIZE DESCRIPTION]
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Genetic Algorithm", meta = (ClampMin = "10", UIMin = "10"))
	int32 PopulationSize = 100;

	/**
	 * [INSERT NUM GENERATIONS DESCRIPTION]
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Genetic Algorithm", meta = (ClampMin = "1", UIMin = "1"))
	int32 NumGenerations = 50;

	/**
	 * [INSERT RANDOM SEED DESCRIPTION}
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Genetic Algorithm")
	int32 GARandomSeed = 12345;

	/**
	 * [INSERT CHROMOSOME LENGTH DESCRIPTION}
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Genetic Algorithm")
	int32 ChromosomeLenght = 5;

	/**
	 * [INSERT USE ELITISM LENGTH DESCRIPTION}
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Genetic Algorithm|Elitism")
	bool bUseElitism = true;

	/**
	 * [INSERT USE ELITISM LENGTH DESCRIPTION}
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Genetic Algorithm|Elitism", meta = (EditCondition = "bUseElitism", ClampMin = "1", UIMin = "1"))
	int32 EliteIndividuals = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced, Category = "Genetic Algorithm|Operators")
	TObjectPtr<UGASelectionStrategyAbstract> SelectionStrategy = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced, Category = "Genetic Algorithm|Operators")
	TObjectPtr<UGACrossoverStrategyAbstract> CrossoverStrategy = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced, Category = "Genetic Algorithm|Operators")
	TObjectPtr<UGAMutationStrategyAbstract> MutationStrategy = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced, Category = "Genetic Algorithm|Evaluation")
	TObjectPtr<UGAFitnessStrategyAbstract> FitnessStrategy = nullptr;
};
