// © 2025 Beatriz Sabater Serna <dev@beatrizsabater.es>


#include "GA/Fitness/GAFitnessStrategy_WinRatePerformance.h"

#include "Clash/ClashSubsystem.h"
#include "GA/GAOptimizerSubsystem.h"

float UGAFitnessStrategy_WinRatePerformance::CalculateFitness(UObject* WorldContextObject, const FGAIndividual& Individual, const TArray<int32>& CardPool)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!IsValid(World)) return 0.f;

	UGAOptimizerSubsystem* GASubsystem = World->GetSubsystem<UGAOptimizerSubsystem>();
	if (!IsValid(GASubsystem)) return 0.f;

	UGAConfigDataAsset* GAConfigDataAsset = GASubsystem->GetGeneticAlgorithmConfig();
	if (!IsValid(GAConfigDataAsset)) return 0.f;
	
	int32 TotalWins = 0;
	int32 TotalTurns = 0;

	for (int32 i = 0; i < NumSimulationsPerIndividual; ++i)
	{
		FClashResult Result = RunSimulation(WorldContextObject, Individual);

		if (Result.ClashExecutionResult == EClashExecution::Victory)
		{
			TotalWins += 1;
			TotalTurns += Result.PlayedTurns;
		}
	}

	if (NumSimulationsPerIndividual == 0) return 0.0f; // Avoid division by zero

	float TotalWinsAverage = TotalWins / static_cast<float>(NumSimulationsPerIndividual);
	if (TotalWinsAverage == 0.f) return 0.f; // Performance won't be computed if the user lost all the wins
	
	float TotalTurnsAverage = TotalTurns / static_cast<float>(NumSimulationsPerIndividual);
	float Performance = static_cast<float>(OptimalTurnsPerformance) / TotalTurnsAverage; // The 2 turns should be computed
	
	return WinRateWeight * TotalWinsAverage + PerformanceWeight * Performance;
}

FClashResult UGAFitnessStrategy_WinRatePerformance::RunSimulation(UObject* WorldContextObject, const FGAIndividual& Individual)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!World)
	{
		UE_LOG(LogTemp, Error, TEXT("RunSimulation: Could not get World."));
		return FClashResult();
	}

	UClashSubsystem* ClashSubsystem = World->GetSubsystem<UClashSubsystem>();
	if (!IsValid(ClashSubsystem)) return FClashResult();

	FClashResult ClashResult = ClashSubsystem->StartAIClash(Individual.Chromosome);

	return ClashResult;
}