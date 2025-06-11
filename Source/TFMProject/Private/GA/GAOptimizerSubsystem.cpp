// © 2025 Beatriz Sabater Serna <dev@beatrizsabater.es>


#include "GA/GAOptimizerSubsystem.h"

#include "TFMProject/TFMProject.h"
#include "GA/GAConfigDataAsset.h"
#include "GA/GAOperators.h"
#include "GA/GeneticAlgorithmUtils.h"


void UGAOptimizerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

bool UGAOptimizerSubsystem::ValidateConfig() const
{
	if (!IsValid(GAConfigDataAsset))
	{
		UE_LOG(LogGeneticAlgorithm, Error, TEXT("GAOptimizerSubsystem::ValidateConfig - Genetic Algorithm Data Asset has not been set."));
		return false;
	}

	if (GAConfigDataAsset->PopulationSize <= 0 || GAConfigDataAsset->NumGenerations <= 0)
	{
		UE_LOG(LogGeneticAlgorithm, Error, TEXT("GAOptimizerSubsystem::ValidateConfig - Invalid configuration values in %s."), *GAConfigDataAsset->GetName());
		return false;
	}

	// All the strategies are correct
	if (!IsValid(GAConfigDataAsset->SelectionStrategy) || !IsValid(GAConfigDataAsset->CrossoverStrategy) || !IsValid(GAConfigDataAsset->MutationStrategy) || !IsValid(GAConfigDataAsset->FitnessStrategy))
	{
		UE_LOG(LogGeneticAlgorithm, Error, TEXT("GAOptimizerSubsystem::ValidateConfig - Invalid strategies in %s."), *GAConfigDataAsset->GetName());
		return false;
	}

	// TODO: Add a function to the data asset for validating strategies can be done?

	if (CardPool.IsEmpty())
	{
		UE_LOG(LogGeneticAlgorithm, Error, TEXT("GAOptimizerSubsystem::ValidateConfig - Card Pool is empty."));
		return false;
	}

	return true;
}

void UGAOptimizerSubsystem::ResetSystemProperties()
{
	CurrentGeneration = 0;
	BestIndividualFound = FGAIndividual();
	BestIndividualFound.Fitness = -MAX_FLT;
	bStopRequested = false;
	
	CurrentRunLog = FGAReport();
	CurrentRunLog.StartTime = FDateTime::UtcNow();

	CardPool.Empty();
}

void UGAOptimizerSubsystem::StartOptimization(UGAConfigDataAsset* ConfigDataAsset, TArray<int32>& InCardPool)
{
	if (bIsRunning)
	{
		UE_LOG(LogGeneticAlgorithm, Warning, TEXT("GAOptimizerSubsystem::StartOptimization - Optimization already in progress."));
		return;
	}
	
	ResetSystemProperties();

	GAConfigDataAsset = ConfigDataAsset;
	CardPool = InCardPool;
	
	if (!ValidateConfig()) return;
	
	GARandomStream.Initialize(GAConfigDataAsset->GARandomSeed == -1 ? FPlatformTime::Cycles() : GAConfigDataAsset->GARandomSeed);
	
	UE_LOG(LogGeneticAlgorithm, Log, TEXT("GAOptimizerSubsystem::StartOptimization - Starting GA Optimization with config: %s"), *GAConfigDataAsset->GetName());

	bIsRunning = true;
	
	OnOptimizationStartedDelegate.Broadcast(GAConfigDataAsset);
	
	// Generating initial generation
	TArray<FGAIndividual> CurrentPopulation;
	InitializePopulation(CurrentPopulation);

	// --- Bucle Principal (Síncrono) --- TODO: Debería ser asíncrono en un futuro probablemente
	while (CurrentGeneration < GAConfigDataAsset->NumGenerations && !bStopRequested)
	{
		CurrentGeneration++;
		RunSingleGeneration(CurrentPopulation);
	}

	// Finish optimization
	bIsRunning = false;

	OnOptimizationFinishedDelegate.Broadcast(BestIndividualFound, true);
	OnOptimizationCompleteDelegate.Broadcast(BestIndividualFound);
	
	UE_LOG(LogTemp, Log, TEXT("GA Optimization Finished. Best Fitness Found: %f"), BestIndividualFound.Fitness);
	UE_LOG(LogTemp, Log, TEXT("Best Chromosome: [ %s]"), *UGeneticAlgorithmUtils::ChromosomeToString(BestIndividualFound.Chromosome));
}

void UGAOptimizerSubsystem::StopOptimization()
{
	if (bIsRunning && !bStopRequested)
	{
		UE_LOG(LogGeneticAlgorithm, Log, TEXT("GAOptimizerSubsystem::StopOptimization - Optimization stop requested and processed."));
		bStopRequested = true;
	}
	else
	{
		UE_LOG(LogGeneticAlgorithm, Warning, TEXT("GAOptimizerSubsystem::StopOptimization - Optimization stop requested but is not running or the stop was already requested."));
	}
}

void UGAOptimizerSubsystem::RunSingleGeneration(TArray<FGAIndividual>& InPopulation)
{
	if (!ValidateConfig()) return;

	if (InPopulation.IsEmpty())
	{
		UE_LOG(LogGeneticAlgorithm, Error, TEXT("GAOptimizerSubsystem::RunSingleGeneration - Received an empty population."));
		return;
	}

	UE_LOG(LogGeneticAlgorithm, Log, TEXT("GAOptimizerSubsystem::RunSingleGeneration - Running generation %i."), CurrentGeneration);

	OnGenerationStartedDelegate.Broadcast(CurrentGeneration);

	// Reports
	TArray<FGACrossoverReportEntry> CrossoverReportEntries;
	TArray<FGAMutationReportEntry> MutationReportEntries;
	
	// Creating a copy to not override the population until the end
	TArray<FGAIndividual> CurrentPopulation = InPopulation;
	TArray<FGAIndividual> SelectedParents;
	TArray<FGAIndividual> Offspring;

	// 1. Selection
	GAConfigDataAsset->SelectionStrategy->SelectParents(this, CurrentPopulation, SelectedParents);

	// TODO: MAYBE DO A CHECK MACRO AND DO ANOTHER CHECKS (less code)
	if (SelectedParents.IsEmpty())
	{
		UE_LOG(LogGeneticAlgorithm, Error, TEXT("GAOptimizerSubsystem::RunSingleGeneration - Something in the selection of the generation %i went wrong, throwing an empty array."), CurrentGeneration);
		StopOptimization();
		return;
	}

	OnParentsSelectedDelegate.Broadcast(SelectedParents);
	
	// 2. Crossover
	GAConfigDataAsset->CrossoverStrategy->PerformCrossover(this, SelectedParents, Offspring, CrossoverReportEntries);

	// TODO: MAYBE DO A CHECK MACRO AND DO ANOTHER CHECKS (less code)
	if (Offspring.IsEmpty())
	{
		UE_LOG(LogGeneticAlgorithm, Error, TEXT("GAOptimizerSubsystem::RunSingleGeneration - Something in the crossover of the generation %i went wrong, throwing an empty array."), CurrentGeneration);
		StopOptimization();
		return;
	}

	OnCrossoverPerformedDelegate.Broadcast(CrossoverReportEntries);
	
	// 3. Mutation
	GAConfigDataAsset->MutationStrategy->PerformMutation(this, Offspring, CardPool, MutationReportEntries);

	// TODO: MAYBE DO A CHECK MACRO AND DO ANOTHER CHECKS (less code)
	if (Offspring.IsEmpty())
	{
		UE_LOG(LogGeneticAlgorithm, Error, TEXT("GAOptimizerSubsystem::RunSingleGeneration - Something in the mutation of the generation %i went wrong, throwing an empty array."), CurrentGeneration);
		StopOptimization();
		return;
	}

	OnMutationPerformedDelegate.Broadcast(MutationReportEntries);
	
	// 4. Evaluation
	EvaluatePopulation(Offspring);

	// TODO: MAYBE DO A CHECK MACRO AND DO ANOTHER CHECKS (less code)
	if (Offspring.IsEmpty()) return;

	// 5. Elitism
	ApplyElitism(CurrentPopulation, Offspring);
	
	// 6. Update (assuming that Evaluate Population orders by descendent)
	UpdateBestIndividual(InPopulation[0]);

	OnGenerationFinishedDelegate.Broadcast(CurrentGeneration, BestIndividualFound, 0.f, Offspring);
	//OnProgressUpdate.Broadcast(CurrentGeneration, BestIndividualFound.Fitness);

	// 7. Reemplazar la población antigua con la nueva descendencia
	InPopulation.Empty();
	InPopulation.Append(Offspring);    
}

void UGAOptimizerSubsystem::InitializePopulation(TArray<FGAIndividual>& OutPopulation)
{
	if (!ValidateConfig()) return;
	
	const int32 PopulationSize = GAConfigDataAsset->PopulationSize;
	const int32 ChromosomeLenght = GAConfigDataAsset->ChromosomeLenght;

	OutPopulation.Empty(PopulationSize);

	if (CardPool.Num() < ChromosomeLenght)
	{
		UE_LOG(LogGeneticAlgorithm, Error, TEXT("GAOptimizerSubsystem::InitializePopulation - Card Pool has less cards (%i) than chromosome lenght (%i), cannot generate valid individuals."), CardPool.Num(), ChromosomeLenght);
		return; 
	}

	OnGenerationStartedDelegate.Broadcast(0);

	// TODO: THIS CAN BE A STRATEGY IN THE DATA ASSET TO CHOOSE THE INITIAL POPULATION PICK METHOD
	// INDIVIDUALS GENERATION
	for (int32 i = 0; i < PopulationSize; ++i)
	{
		FGAIndividual NewIndividual;
		TArray<int32> AvailableCards = CardPool;

		// CHROMOSOME GENERATION
		for (int32 j = 0; j < ChromosomeLenght; ++j)
		{
			if (AvailableCards.IsEmpty())
			{
				UE_LOG(LogGeneticAlgorithm, Error, TEXT("GAOptimizerSubsystem::InitializePopulation - Ran out of cards during initial individual generation."));
				break;
			}
			int32 RandomIndex = GARandomStream.RandRange(0, AvailableCards.Num() - 1);
			NewIndividual.Chromosome.Add(AvailableCards[RandomIndex]);
			AvailableCards.RemoveAtSwap(RandomIndex); // Eficiente para quitar
		}

		if (NewIndividual.Chromosome.Num() != ChromosomeLenght)
		{
			UE_LOG(LogGeneticAlgorithm, Error, TEXT("GAOptimizerSubsystem::InitializePopulation - Failed to generate a valid chromosome for an initial individual."));
			StopOptimization();
			return;
		}
		
		OutPopulation.Add(NewIndividual);
	}

	if (OutPopulation.IsEmpty())
	{
		UE_LOG(LogGeneticAlgorithm, Error, TEXT("GAOptimizerSubsystem::InitializePopulation - Failed to generate a valid chromosome for an initial individual."));
		StopOptimization();
		return;
	}

	OnParentsSelectedDelegate.Broadcast(OutPopulation);

	UE_LOG(LogGeneticAlgorithm, Log, TEXT("GAOptimizerSubsystem::InitializePopulation - Initial population of size %d generated."), OutPopulation.Num());

	// Evaluate initial population
	EvaluatePopulation(OutPopulation);

	// Update best individual
	if (!OutPopulation.IsEmpty())
	{
		UpdateBestIndividual(OutPopulation[0]); 
		OnProgressUpdateDelegate.Broadcast(0, BestIndividualFound.Fitness);
	}

	OnGenerationFinishedDelegate.Broadcast(0, BestIndividualFound, 0.f, OutPopulation);

	for (auto PopulationIndividual : OutPopulation)
	{
		UE_LOG(LogGeneticAlgorithm, Verbose, TEXT("[ %s ], %f"), *UGeneticAlgorithmUtils::ChromosomeToString(BestIndividualFound.Chromosome), PopulationIndividual.Fitness);
	}
}

void UGAOptimizerSubsystem::EvaluatePopulation(TArray<FGAIndividual>& InPopulation)
{
	if (!IsValid(GAConfigDataAsset))
	{
		UE_LOG(LogGeneticAlgorithm, Error, TEXT("GAOptimizerSubsystem::EvaluatePopulation - GAConfigDataAsset is not valid."));
		return;
	}
	
	if (InPopulation.IsEmpty())
	{
		UE_LOG(LogGeneticAlgorithm, Error, TEXT("GAOptimizerSubsystem::EvaluatePopulation - Population is empty."));
		return;
	}

	 // TODO: Paralelizacion potencial aqui si CalculateFitness es el cuello de botella. Por ahora, secuencial:
	for (FGAIndividual& Individual : InPopulation)
	{
		Individual.Fitness = GAConfigDataAsset->FitnessStrategy->CalculateFitness(this, Individual, CardPool);
	}

	// Fitness order
	InPopulation.Sort([](const FGAIndividual& A, const FGAIndividual& B) {
		return A.Fitness > B.Fitness;
	});
}

void UGAOptimizerSubsystem::FinishOptimization()
{
	bIsRunning = false;
	UE_LOG(LogTemp, Log, TEXT("GA Optimization Finished. Best Fitness Found: %f"), BestIndividualFound.Fitness);
	UE_LOG(LogTemp, Log, TEXT("Best Chromosome: [ %s]"), *UGeneticAlgorithmUtils::ChromosomeToString(BestIndividualFound.Chromosome));

	//OnGenerationFinishedDelegate.Broadcast(CurrentGeneration, BestIndividualFound, 0.f, )
	OnOptimizationCompleteDelegate.Broadcast(BestIndividualFound);
}

void UGAOptimizerSubsystem::ApplyElitism(const TArray<FGAIndividual>& CurrentPopulation, TArray<FGAIndividual>& Offspring)
{
	if (!IsValid(GAConfigDataAsset) || !GAConfigDataAsset->bUseElitism || GAConfigDataAsset->EliteIndividuals <= 0)
	{
		UE_LOG(LogGeneticAlgorithm, Verbose, TEXT("GAOptimizerSubsystem::ApplyElitism - Elitism disabled, using only offspring."));
		return;
	}
	
	int32 EliteCount = FMath::Min(GAConfigDataAsset->EliteIndividuals, Offspring.Num());
	
	UE_LOG(LogGeneticAlgorithm, Verbose, TEXT("GAOptimizerSubsystem::ApplyElitism - Applying elitism with %i elite individuals."), EliteCount);
	
	int32 OffspringToKeep = Offspring.Num() - EliteCount;
    
	if (OffspringToKeep < Offspring.Num())
	{
		Offspring.RemoveAt(OffspringToKeep , Offspring.Num() - OffspringToKeep);
	}
    
	for (int32 i = 0; i < EliteCount; i++)
	{
		Offspring.Add(CurrentPopulation[i]);
	}

	Offspring.Sort([](const FGAIndividual& A, const FGAIndividual& B) {
		return A.Fitness > B.Fitness;
	});
}

void UGAOptimizerSubsystem::UpdateBestIndividual(const FGAIndividual& Candidate)
{
	if (Candidate.Fitness > BestIndividualFound.Fitness)
	{
		BestIndividualFound = Candidate;
	}
}
