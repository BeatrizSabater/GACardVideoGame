// © 2025 Beatriz Sabater Serna <dev@beatrizsabater.es>


#include "GA/Mutation/GAMutationStrategy_UniqueGeneSwap.h"

#include "GA/GAConfigDataAsset.h"
#include "GA/GAOptimizerSubsystem.h"
#include "TFMProject/TFMProject.h"

void UGAMutationStrategy_UniqueGeneSwap::PerformMutation(UObject* WorldContextObject, TArray<FGAIndividual>& PopulationInOut, const TArray<int32>& CardPool, TArray<FGAMutationReportEntry>& OutReportEntries)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!IsValid(World)) return;
	
	UGAOptimizerSubsystem* GASubsystem = World->GetSubsystem<UGAOptimizerSubsystem>();
	if (!IsValid(GASubsystem)) return;
	
	UGAConfigDataAsset* GAConfigDataAsset = GASubsystem->GetGeneticAlgorithmConfig();
	if (!IsValid(GAConfigDataAsset)) return;
	
	if (PopulationInOut.IsEmpty())
	{
		UE_LOG(LogGeneticAlgorithm, Warning, TEXT(" UGAMutationStrategy_UniqueGeneSwap::PerformMutation - Empty population."));
		return;
	}

	for (FGAIndividual& Individual : PopulationInOut)
	{
		if (GASubsystem->GetGARandomStream().FRand() < this->MutationRate)
		{
			if (Individual.Chromosome.Num() != GASubsystem->GetGeneticAlgorithmConfig()->ChromosomeLenght)
			{
				UE_LOG(LogGeneticAlgorithm, Error, TEXT("UGAMutationStrategy_UniqueGeneSwap::PerformMutation - Individual chromosome lenght (%i) isn't the same as the config (%i)."), Individual.Chromosome.Num(), GASubsystem->GetGeneticAlgorithmConfig()->ChromosomeLenght);
				continue;
			}

			FGAMutationReportEntry MutationReportEntry;
			MutationReportEntry.ChromosomeBefore = Individual.Chromosome;

			const int32 IndexToReplace = GASubsystem->GetGARandomStream().RandRange(0, Individual.Chromosome.Num() - 1);

			int32 NewGene = -1;
			int32 Attempts = 0;
			const int32 MaxAttempts = CardPool.Num() * 2;
			while (Attempts < MaxAttempts)
			{
				NewGene = CardPool[GASubsystem->GetGARandomStream().RandRange(0, CardPool.Num() - 1)];
				
				if (!Individual.Chromosome.Contains(NewGene)) break;
				
				NewGene = -1;
				Attempts++;
			}

			if (NewGene != -1)
			{
				Individual.Chromosome[IndexToReplace] = NewGene;
				MutationReportEntry.ChromosomeAfter = Individual.Chromosome;
				OutReportEntries.Emplace(MutationReportEntry);
			}
			else
			{
				UE_LOG(LogGeneticAlgorithm, Error, TEXT("UGAMutationStrategy_UniqueGeneSwap::PerformMutation - Mutation over individual couldn't be done."));
			}
		}
	}
}
