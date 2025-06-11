// © 2025 Beatriz Sabater Serna <dev@beatrizsabater.es>


#include "GA/Fitness/GAFitnessStrategy_WRP_TurnsHealth.h"

#include "GA/GAConfigDataAsset.h"
#include "GA/GAOptimizerSubsystem.h"
#include "Units/UnitUtils.h"
#include "Units/Modifiers/UnitModifier_Shell.h"

float UGAFitnessStrategy_WRP_TurnsHealth::CalculateFitness(UObject* WorldContextObject, const FGAIndividual& Individual, const TArray<int32>& CardPool)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!IsValid(World)) return 0.f;

	UGAOptimizerSubsystem* GASubsystem = World->GetSubsystem<UGAOptimizerSubsystem>();
	if (!IsValid(GASubsystem)) return 0.f;

	UGAConfigDataAsset* GAConfigDataAsset = GASubsystem->GetGeneticAlgorithmConfig();
	if (!IsValid(GAConfigDataAsset)) return 0.f;

	UClashSubsystem* ClashSubsystem = World->GetSubsystem<UClashSubsystem>();
	if (!IsValid(ClashSubsystem)) return 0.f;
	
	int32 TotalWins = 0;
	float TotalPerformance = 0.f;

	const int32 RunningSimulations = ClashSubsystem->GetClashUnitsCombinations() * NumSimulationsPerIndividual;

	for (int32 i = 0; i < RunningSimulations; ++i)
	{
		FClashResult Result = RunSimulation(WorldContextObject, Individual, i);

		if (Result.ClashExecutionResult == EClashExecution::Victory)
		{
			TotalWins += 1;
			TotalPerformance += CalculatePerformanceByClash(Result.PlayedTurns, Result.AlivePlayerUnits[0]);
		}
	}

	if (RunningSimulations == 0) return 0.0f; // Avoid division by zero

	float TotalWinsAverage = TotalWins / static_cast<float>(RunningSimulations);
	if (TotalWinsAverage == 0.f) return 0.f; // Performance won't be computed if the user lost all the wins
	
	float PerformanceWeight = 1.0f - WinRateWeight;
	float AveragePerformance = TotalPerformance / static_cast<float>(RunningSimulations);
	
	return WinRateWeight * TotalWinsAverage + PerformanceWeight * AveragePerformance;
}

FClashResult UGAFitnessStrategy_WRP_TurnsHealth::RunSimulation(UObject* WorldContextObject, const FGAIndividual& Individual, int32 ConsecutiveExecutionNumber)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!World)
	{
		UE_LOG(LogTemp, Error, TEXT("RunSimulation: Could not get World."));
		return FClashResult();
	}

	UClashSubsystem* ClashSubsystem = World->GetSubsystem<UClashSubsystem>();
	if (!IsValid(ClashSubsystem)) return FClashResult();

	FClashResult ClashResult = ClashSubsystem->StartAIClash(Individual.Chromosome, ConsecutiveExecutionNumber);

	return ClashResult;
}

float UGAFitnessStrategy_WRP_TurnsHealth::CalculatePerformanceByClash(int32 PlayedTurns, FUnitStats PlayerUnitStats)
{
	float PerformanceHealthWeight = 1.f - PerformanceTurnsWeight;
	
	float TurnsPerformance = static_cast<float>(OptimalTurnsPerformance) / PlayedTurns;
	TurnsPerformance = FMath::Clamp(TurnsPerformance, 0.f, 1.f);

	int32 CurrentHealthWithShell = PlayerUnitStats.CurrentHealth;
	
	if (UUnitModifierAbstract* ShellUnitModifier = UUnitUtils::GetUnitModifierByClass(UUnitModifier_Shell::StaticClass(), PlayerUnitStats))
	{
		CurrentHealthWithShell = FMath::Min(PlayerUnitStats.Health, CurrentHealthWithShell + ShellUnitModifier->AppliedUnitModifier.ModifierPoints);
	}
	
	float HealthPerformance = static_cast<float>(CurrentHealthWithShell) / static_cast<float>(PlayerUnitStats.Health);
	HealthPerformance = FMath::Clamp(HealthPerformance, 0.0f, 1.0f);

	return HealthPerformance * PerformanceHealthWeight + TurnsPerformance * PerformanceTurnsWeight;
}
