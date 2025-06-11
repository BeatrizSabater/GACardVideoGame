// © 2025 Beatriz Sabater Serna <dev@beatrizsabater.es>

#pragma once

#include "CoreMinimal.h"
#include "GAConfigDataAsset.h"

#include "Engine/DataAsset.h"
#include "GA/GAOperators.h"
#include "GeneticAlgorithmReport.generated.h"

class UGAConfigDataAsset;


USTRUCT(BlueprintType)
struct TFMPROJECT_API FGACrossoverReportEntry
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<int32> Parent1Chromosome;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<int32> Parent2Chromosome; // Can be empty if its cloned

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<int32> Offspring1Chromosome;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<int32> Offspring2Chromosome; // Can be empty
};

USTRUCT(BlueprintType)
struct TFMPROJECT_API FGAMutationReportEntry
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<int32> ChromosomeBefore;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<int32> ChromosomeAfter;
};


USTRUCT(BlueprintType)
struct TFMPROJECT_API FGAGenerationReport
{
	GENERATED_BODY()
	
	FGAGenerationReport(){}

	FGAGenerationReport(const int32 InGenerationNumber) : GenerationNumber(InGenerationNumber){}

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 GenerationNumber = -1;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FGAIndividual BestIndividualOfGeneration;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float AverageFitness = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FGAIndividual> SelectedParentChromosomes;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FGACrossoverReportEntry> CrossoverEvents;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FGAMutationReportEntry> MutationEvents;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FGAIndividual> EvaluatedPopulation;
};

USTRUCT(BlueprintType)
struct TFMPROJECT_API FGARunParameters
{
	GENERATED_BODY()

	FGARunParameters(){}

	FGARunParameters(const UGAConfigDataAsset* GAConfigDataAsset)
	{
		if (!IsValid(GAConfigDataAsset)) return;

		GADataAssetName = GAConfigDataAsset->GetName();
		PopulationSize = GAConfigDataAsset->PopulationSize;
		NumGenerations = GAConfigDataAsset->NumGenerations;
		RandomSeed = GAConfigDataAsset->GARandomSeed;
		
		SelectionStrategyClass = GAConfigDataAsset->SelectionStrategy ? GAConfigDataAsset->SelectionStrategy->GetClass()->GetName() : TEXT("Error in class name.");
		CrossoverStrategyClass = GAConfigDataAsset->CrossoverStrategy ? GAConfigDataAsset->CrossoverStrategy->GetClass()->GetName() : TEXT("Error in class name.");
		MutationStrategyClass = GAConfigDataAsset->MutationStrategy ? GAConfigDataAsset->MutationStrategy->GetClass()->GetName() : TEXT("Error in class name.");
		FitnessStrategyClass = GAConfigDataAsset->FitnessStrategy ? GAConfigDataAsset->FitnessStrategy->GetClass()->GetName() : TEXT("Error in class name.");
	}

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString GADataAssetName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 PopulationSize = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 NumGenerations = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 RandomSeed = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString SelectionStrategyClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString CrossoverStrategyClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString MutationStrategyClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString FitnessStrategyClass;
};


USTRUCT(BlueprintType)
struct TFMPROJECT_API FGAReport
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="GA Log")
	FDateTime StartTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="GA Log")
	FDateTime EndTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="GA Log")
	FTimespan Duration;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FGARunParameters RunParameters;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="GA Log")
	TArray<FGAGenerationReport> GenerationLogs;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="GA Log")
	FGAIndividual OverallBestIndividual;
};