// © 2025 Beatriz Sabater Serna <dev@beatrizsabater.es>


#include "..\..\..\Public\GA\Fitness\GAFitnessStrategy_WinRate.h"

#include "GA/GAConfigDataAsset.h"
#include "GA/GAOptimizerSubsystem.h"

float UGAFitnessStrategy_WinRate::CalculateFitness(UObject* WorldContextObject, const FGAIndividual& Individual, const TArray<int32>& CardPool)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!IsValid(World)) return 0.f;

	UGAOptimizerSubsystem* GASubsystem = World->GetSubsystem<UGAOptimizerSubsystem>();
	if (!IsValid(GASubsystem)) return 0.f;

	UGAConfigDataAsset* GAConfigDataAsset = GASubsystem->GetGeneticAlgorithmConfig();
	if (!IsValid(GAConfigDataAsset)) return 0.f;
	
	int32 TotalWins = 0;

	for (int32 i = 0; i < NumSimulationsPerIndividual; ++i)
	{
		FClashResult Result = RunSimulation(WorldContextObject, Individual);

		if (Result.ClashExecutionResult == EClashExecution::Victory)
		{
			TotalWins += 1;
		}
	}

	if (NumSimulationsPerIndividual == 0) return 0.0f; // Avoid division by zero
	
	return TotalWins / static_cast<float>(NumSimulationsPerIndividual);
}

FClashResult UGAFitnessStrategy_WinRate::RunSimulation(UObject* WorldContextObject, const FGAIndividual& Individual)
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