// © 2025 Beatriz Sabater Serna <dev@beatrizsabater.es>

#pragma once

#include "CoreMinimal.h"
#include "GeneticAlgorithmReport.h"
#include "GA/GAOperators.h"
#include "Subsystems/WorldSubsystem.h"
#include "GAOptimizerSubsystem.generated.h"

class UGAConfigDataAsset;
class UProt_GeneticAlgorithmConfig;

USTRUCT(BlueprintType)
struct TFMPROJECT_API FGeneticAlgorithmResult
{
	GENERATED_BODY()

	FGeneticAlgorithmResult(){}

	FGeneticAlgorithmResult(const FGAIndividual& InBestIndividual)
	: BestIndividual(InBestIndividual)
	{}

	UPROPERTY(BlueprintReadOnly)
	FGAIndividual BestIndividual;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGAOptimizationCompleteSignature, const FGAIndividual&, BestIndividual);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGAOptimizationProgressSignature, int32, CurrentGeneration, float, BestFitness);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGAOptimizationStartedSignature, const UGAConfigDataAsset*, RunParams);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGAOptimizationFinishedSignature, const FGAIndividual&, OverallBestIndividual, bool, bCompletedSuccessfully);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGAOptimizationGenerationStartedSignature, const int32, GenerationNumber);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FGAOptimizationGenerationFinishedSignature, int32, GenerationNumber, const FGAIndividual&, BestOfGeneration, float, AverageFitness, const TArray<FGAIndividual>&, FinalPopulation);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGAParentsSelectedSignature, const TArray<FGAIndividual>&, SelectedParents);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGACrossoverPerformedSignature, const TArray<FGACrossoverReportEntry>&, CrossoverEvents);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGAMutationPerformedSignature, const TArray<FGAMutationReportEntry>&, MutationEvents);

UCLASS()
class TFMPROJECT_API UGAOptimizerSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	/////////////////////////////// OPTIMIZATION OPERATIONAL FUNCTIONS ///////////////////////////////
	UFUNCTION(BlueprintCallable, Category = "Genetic Algorithm")
	void StartOptimization(UGAConfigDataAsset* ConfigDataAsset, TArray<int32>& InCardPool);

	UFUNCTION(BlueprintCallable, Category = "Genetic Algorithm")
	void StopOptimization();

	UFUNCTION(BlueprintPure, Category = "Genetic Algorithm")
	bool IsRunning() const { return bIsRunning; }
	//////////////////////////////////////////////////////////////////////////////////////////////////

	
	/////////////////////////////// DELEGATES ///////////////////////////////
	UPROPERTY(BlueprintAssignable, Category = "Genetic Algorithm | Events")
	FGAOptimizationCompleteSignature OnOptimizationCompleteDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Genetic Algorithm | Events")
	FGAOptimizationProgressSignature OnProgressUpdateDelegate;
	/////////////////////////////////////////////////////////////////////////
	
	UFUNCTION(BlueprintCallable)
	void SetGeneticAlgorithmConfig(UGAConfigDataAsset* InGAConfigDataAsset) { GAConfigDataAsset = InGAConfigDataAsset; };

	UFUNCTION(BlueprintCallable)
	UGAConfigDataAsset* GetGeneticAlgorithmConfig() const { return GAConfigDataAsset; };

	UFUNCTION(BlueprintCallable)
	const FRandomStream& GetGARandomStream() const { return GARandomStream; };

	/////////////////////////////// DELEGATES ///////////////////////////////
	UPROPERTY(BlueprintAssignable, Category = "Genetic Algorithm | Events")
	FGAOptimizationStartedSignature OnOptimizationStartedDelegate;
	UPROPERTY(BlueprintAssignable, Category = "Genetic Algorithm | Events")
	FGAOptimizationFinishedSignature OnOptimizationFinishedDelegate;
	UPROPERTY(BlueprintAssignable, Category = "Genetic Algorithm | Events")
	FGAOptimizationGenerationStartedSignature OnGenerationStartedDelegate;
	UPROPERTY(BlueprintAssignable, Category = "Genetic Algorithm | Events")
	FGAParentsSelectedSignature OnParentsSelectedDelegate;
	UPROPERTY(BlueprintAssignable, Category = "Genetic Algorithm | Events")
	FGACrossoverPerformedSignature OnCrossoverPerformedDelegate;
	UPROPERTY(BlueprintAssignable, Category = "Genetic Algorithm | Events")
	FGAMutationPerformedSignature OnMutationPerformedDelegate;
	UPROPERTY(BlueprintAssignable, Category = "Genetic Algorithm | Events")
	FGAOptimizationGenerationFinishedSignature OnGenerationFinishedDelegate;
	/////////////////////////////////////////////////////////////////////////

private:
	// Algorithm state
	int32 CurrentGeneration = 0;
	FGAIndividual BestIndividualFound;
	bool bIsRunning = false;
	bool bStopRequested = false;
	FRandomStream GARandomStream;
	UPROPERTY()
	TObjectPtr<UGAConfigDataAsset> GAConfigDataAsset = nullptr;

	// --- Algorithm phases ---
	void InitializePopulation(TArray<FGAIndividual>& OutPopulation);
	void RunSingleGeneration(TArray<FGAIndividual>& InPopulation);
	void EvaluatePopulation(TArray<FGAIndividual>& InPopulation);
	void FinishOptimization();
	
	TArray<int32> CardPool;

	bool ValidateConfig() const;
	void ResetSystemProperties();
	void UpdateBestIndividual(const FGAIndividual& Candidate);
	void ApplyElitism(const TArray<FGAIndividual>& CurrentPopulation, TArray<FGAIndividual>& Offspring);

	UPROPERTY()
	FGAReport CurrentRunLog;
};
