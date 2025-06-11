// © 2025 Beatriz Sabater Serna <dev@beatrizsabater.es>


#include "GA/GAReporterSubsystem.h"

#include "GA/GAOptimizerSubsystem.h"
#include "TFMProject/TFMProject.h"

void UGAReporterSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	Collection.InitializeDependency<UGAOptimizerSubsystem>();

	const UWorld* World = GetWorld();
	if (World == nullptr)
	{
		UE_LOG(LogGeneticAlgorithmReporter, Error, TEXT("UGAReporterSubsystem::Initialize - GetWorld() returned null during initialize.!"));
		return;
	}

	OptimizerSubsystemRef = World->GetSubsystem<UGAOptimizerSubsystem>();
	if (OptimizerSubsystemRef == nullptr)
	{
		UE_LOG(LogGeneticAlgorithmReporter, Error, TEXT("UGAReporterSubsystem::Initialize - Failed to get GAOptimizerSubsystem even after declaring dependency."));
		return;
	}

	SubscribeToDelegates();
}

void UGAReporterSubsystem::Deinitialize()
{
	UnsubscribeFromDelegates();
	
	Super::Deinitialize();
}

void UGAReporterSubsystem::HandleOptimizationStarted(const UGAConfigDataAsset* RunParams)
{
	UE_LOG(LogGeneticAlgorithmReporter, Log, TEXT("UGAReporterSubsystem::HandleOptimizationStarted."));
	
	ResetReport();
	
	bIsRunningReport = true;
	CurrentRunReport.StartTime = FDateTime::UtcNow();
	CurrentRunReport.RunParameters = FGARunParameters(RunParams);
}

void UGAReporterSubsystem::HandleOptimizationFinished(const FGAIndividual& OverallBestIndividual, bool bCompletedSuccessfully)
{
	if (!bIsRunningReport) return;

	UE_LOG(LogGeneticAlgorithmReporter, Log, TEXT("UGAReporterSubsystem::HandleOptimizationFinished - Success: %s"), bCompletedSuccessfully ? TEXT("Yes") : TEXT("No"));
	CurrentRunReport.EndTime = FDateTime::UtcNow();
	CurrentRunReport.Duration = CurrentRunReport.EndTime - CurrentRunReport.StartTime;
	CurrentRunReport.OverallBestIndividual = OverallBestIndividual;

	bIsRunningReport = false;
	
	SaveReport("LastRun");
}

void UGAReporterSubsystem::HandleGenerationStarted(const int32 GenerationNumber)
{
	if (!bIsRunningReport) return;

	UE_LOG(LogGeneticAlgorithmReporter, Log, TEXT("UGAReporterSubsystem::HandleGenerationStarted - Detected Generation Started: %i"), GenerationNumber);

	ActiveGenerationReport = FGAGenerationReport(GenerationNumber);
}

void UGAReporterSubsystem::HandleParentsSelected(const TArray<FGAIndividual>& SelectedParents)
{
	if (!bIsRunningReport || ActiveGenerationReport.GenerationNumber == -1) return;
	
	UE_LOG(LogGeneticAlgorithmReporter, Log, TEXT("UGAReporterSubsystem::HandleParentsSelected - Detected Parents Selected for Gen %i. Count: %i"), ActiveGenerationReport.GenerationNumber, SelectedParents.Num());
	
	ActiveGenerationReport.SelectedParentChromosomes = SelectedParents;
}

void UGAReporterSubsystem::HandleCrossoverPerformed(const TArray<FGACrossoverReportEntry>& CrossoverEvents)
{
	if (!bIsRunningReport || ActiveGenerationReport.GenerationNumber == -1) return;
	
	UE_LOG(LogGeneticAlgorithmReporter, Log, TEXT("UGAReporterSubsystem::HandleCrossoverPerformed - Detected Crossover Performed for Gen %i. Events: %i"), ActiveGenerationReport.GenerationNumber, CrossoverEvents.Num());

	ActiveGenerationReport.CrossoverEvents = CrossoverEvents;
}

void UGAReporterSubsystem::HandleMutationPerformed(const TArray<FGAMutationReportEntry>& MutationEvents)
{
	if (!bIsRunningReport || ActiveGenerationReport.GenerationNumber == -1) return;
	
	UE_LOG(LogGeneticAlgorithmReporter, Verbose, TEXT("UGAReporterSubsystem::HandleMutationPerformed - Detected Mutation Performed for Gen %d. Events: %d"), ActiveGenerationReport.GenerationNumber, MutationEvents.Num());
	
	ActiveGenerationReport.MutationEvents = MutationEvents;
}

void UGAReporterSubsystem::HandleGenerationFinished(int32 GenerationNumber, const FGAIndividual& BestOfGeneration, float AverageFitness, const TArray<FGAIndividual>& FinalPopulation)
{
	if (!bIsRunningReport || ActiveGenerationReport.GenerationNumber == -1) return;

	if (ActiveGenerationReport.GenerationNumber != GenerationNumber)
	{
		UE_LOG(LogGeneticAlgorithmReporter, Error, TEXT("UGAReporterSubsystem::HandleGenerationFinished - Mismatch between active log pointer gen (%d) and finished gen event (%d)."), ActiveGenerationReport.GenerationNumber, GenerationNumber);
		//TODO: STOP REPORT WITH ERROR
		return;
	}

	UE_LOG(LogGeneticAlgorithmReporter, Log, TEXT("UGAReporterSubsystem::HandleGenerationFinished -  Detected Generation Finished: %i. Best Fitness: %f, Avg Fitness: %f"), GenerationNumber, BestOfGeneration.Fitness, AverageFitness);
	
	ActiveGenerationReport.BestIndividualOfGeneration = BestOfGeneration;
	ActiveGenerationReport.AverageFitness = AverageFitness;
	ActiveGenerationReport.EvaluatedPopulation = FinalPopulation;

	CurrentRunReport.GenerationLogs.Emplace(ActiveGenerationReport);
}

bool UGAReporterSubsystem::SaveReport(const FString& SlotName, int32 UserIndex)
{
	return false;
}

bool UGAReporterSubsystem::LoadReport(const FString& SlotName, int32 UserIndex)
{
	return false;
}

void UGAReporterSubsystem::SubscribeToDelegates()
{
	if (!OptimizerSubsystemRef)
	{
		UE_LOG(LogGeneticAlgorithmReporter, Error, TEXT("UGALoggingSubsystem::SubscribeToDelegates - OptimizerSubsystemRef is null. Cannot subscribe."));
		return;
	}
	
	OptimizerSubsystemRef->OnOptimizationStartedDelegate.AddUniqueDynamic(this, &UGAReporterSubsystem::HandleOptimizationStarted);
	OptimizerSubsystemRef->OnOptimizationFinishedDelegate.AddUniqueDynamic(this, &UGAReporterSubsystem::HandleOptimizationFinished);
	OptimizerSubsystemRef->OnGenerationStartedDelegate.AddUniqueDynamic(this, &UGAReporterSubsystem::HandleGenerationStarted);
	OptimizerSubsystemRef->OnGenerationFinishedDelegate.AddUniqueDynamic(this, &UGAReporterSubsystem::HandleGenerationFinished);
	OptimizerSubsystemRef->OnParentsSelectedDelegate.AddUniqueDynamic(this, &UGAReporterSubsystem::HandleParentsSelected);
	OptimizerSubsystemRef->OnCrossoverPerformedDelegate.AddUniqueDynamic(this, &UGAReporterSubsystem::HandleCrossoverPerformed);
	OptimizerSubsystemRef->OnMutationPerformedDelegate.AddUniqueDynamic(this, &UGAReporterSubsystem::HandleMutationPerformed);
}

void UGAReporterSubsystem::UnsubscribeFromDelegates()
{
	if (!OptimizerSubsystemRef)
	{
		UE_LOG(LogGeneticAlgorithmReporter, Error, TEXT("UGALoggingSubsystem::UnsubscribeFromDelegates - OptimizerSubsystemRef is null. Cannot unsubscribe."));
		return;
	}

	OptimizerSubsystemRef->OnOptimizationStartedDelegate.RemoveAll(this);
	OptimizerSubsystemRef->OnOptimizationFinishedDelegate.RemoveAll(this);
	OptimizerSubsystemRef->OnGenerationStartedDelegate.RemoveAll(this);
	OptimizerSubsystemRef->OnGenerationFinishedDelegate.RemoveAll(this);
	OptimizerSubsystemRef->OnParentsSelectedDelegate.RemoveAll(this);
	OptimizerSubsystemRef->OnCrossoverPerformedDelegate.RemoveAll(this);
	OptimizerSubsystemRef->OnMutationPerformedDelegate.RemoveAll(this);
}

void UGAReporterSubsystem::ResetReport()
{
	bIsRunningReport = false;
	CurrentRunReport = FGAReport();
	ActiveGenerationReport = FGAGenerationReport();
}
