// © 2025 Beatriz Sabater Serna <dev@beatrizsabater.es>

#pragma once

#include "CoreMinimal.h"
#include "GAOperators.generated.h"

struct FGAMutationReportEntry;
struct FGACrossoverReportEntry;

USTRUCT(BlueprintType)
struct TFMPROJECT_API FGAIndividual
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> Chromosome;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Fitness = 0.0f;

	FGAIndividual() {}

	explicit FGAIndividual(const TArray<int32>& InChromosome) :
	Chromosome(InChromosome)
	{}

	bool operator<(const FGAIndividual& Other) const
	{
		return Fitness < Other.Fitness;
	}
	bool operator>(const FGAIndividual& Other) const
	{
		return Fitness > Other.Fitness;
	}
};

UCLASS(BlueprintType, EditInlineNew, Abstract)
class TFMPROJECT_API UGASelectionStrategyAbstract : public UObject
{
	GENERATED_BODY()
	
public:
	virtual void SelectParents(UObject* WorldContextObject, const TArray<FGAIndividual>& OldPopulation, TArray<FGAIndividual>& OutSelectedParents) PURE_VIRTUAL(UGASelectionStrategyAbstract::SelectParents,);
};


UCLASS(BlueprintType, EditInlineNew, Abstract)
class TFMPROJECT_API UGAFitnessStrategyAbstract : public UObject
{
	GENERATED_BODY()

public:

	virtual float CalculateFitness(UObject* WorldContextObject, const FGAIndividual& Individual, const TArray<int32>& CardPool) PURE_VIRTUAL(UGAFitnessStrategyAbstract::CalculateFitness, return 0.f;);
};

UCLASS(BlueprintType, EditInlineNew, Abstract)
class TFMPROJECT_API UGACrossoverStrategyAbstract : public UObject
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float CrossoverRate = 0.8f;
	
	virtual void PerformCrossover(const UObject* WorldContextObject, const TArray<FGAIndividual>& Parents, TArray<FGAIndividual>& OutOffspring, TArray<FGACrossoverReportEntry>& OutReportEntries) PURE_VIRTUAL(UGACrossoverStrategyAbstract::Crossover,);
};

UCLASS(BlueprintType, EditInlineNew, Abstract)
class TFMPROJECT_API UGAMutationStrategyAbstract : public UObject
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float MutationRate = 0.1f;
	
	virtual void PerformMutation(UObject* WorldContextObject, TArray<FGAIndividual>& PopulationInOut, const TArray<int32>& CardPool, TArray<FGAMutationReportEntry>& OutReportEntries) PURE_VIRTUAL(UGAMutationStrategyAbstract::PerfomMutation, );
};
