// © 2025 Beatriz Sabater Serna <dev@beatrizsabater.es>


#include "..\..\..\Public\GA\Crossover\GACrossoverStrategy_PickRandomParentsSet.h"

#include "GA/GAConfigDataAsset.h"
#include "GA/GAOptimizerSubsystem.h"
#include "TFMProject/TFMProject.h"

void UGACrossoverStrategy_PickRandomParentsSet::PerformCrossover(const UObject* WorldContextObject, const TArray<FGAIndividual>& Parents, TArray<FGAIndividual>& OutOffspring, TArray<FGACrossoverReportEntry>& OutReportEntries)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!IsValid(World)) return;
	
	UGAOptimizerSubsystem* GASubsystem = World->GetSubsystem<UGAOptimizerSubsystem>();
	if (!IsValid(GASubsystem)) return;
	
	UGAConfigDataAsset* GAConfigDataAsset = GASubsystem->GetGeneticAlgorithmConfig();
	if (!IsValid(GAConfigDataAsset)) return;
	
	if (Parents.IsEmpty())
	{
		UE_LOG(LogGeneticAlgorithm, Warning, TEXT("UGACrossoverStrategy_PickRandomParentsSet:PerformCrossover - Invalid config or empty parent population."));
		return;
	}
	
	const int32 NumParents = Parents.Num();
	const int32 TargetOffspringSize = GAConfigDataAsset->PopulationSize;
	OutOffspring.Empty(TargetOffspringSize);

	FGACrossoverReportEntry CrossoverReportEntry;
	
	for (int32 i = 0; i < TargetOffspringSize; i += 2)
	{
		// Parent selection for this crossover
		const FGAIndividual& Parent1 = Parents[i % NumParents];
		CrossoverReportEntry.Parent1Chromosome = Parent1.Chromosome;
		const FGAIndividual& Parent2 = Parents[(i + 1) % NumParents];
		CrossoverReportEntry.Parent2Chromosome = Parent2.Chromosome;
    
		// Generating a random number to determine whether to crossover or copy the parents
		if (GASubsystem->GetGARandomStream().FRand() < CrossoverRate)
		{
			// Generating first child
			FGAIndividual Child1 = GenerateOffspring(Parent1, Parent2, GASubsystem);
			OutOffspring.Add(Child1);
			CrossoverReportEntry.Offspring1Chromosome = Child1.Chromosome;

			// Generating second child only if there is space left in the offspring population
			if (OutOffspring.Num() < TargetOffspringSize)
			{
				FGAIndividual Child2 = GenerateOffspring(Parent1, Parent2, GASubsystem);
				OutOffspring.Add(Child2);
				CrossoverReportEntry.Offspring2Chromosome = Child2.Chromosome;
			}
		}
		else
		{
			CopyParentsToOffspring(Parent1, Parent2, OutOffspring, CrossoverReportEntry, TargetOffspringSize);
		}

		OutReportEntries.Emplace(CrossoverReportEntry);
	}
	
	// Ensuring target population size. If there is space left in the population, random parents will be picked.
	while (OutOffspring.Num() < TargetOffspringSize && Parents.Num() > 0)
	{
		const int32 RandomParentIndex = GASubsystem->GetGARandomStream().RandRange(0, Parents.Num() - 1);
		OutOffspring.Add(Parents[RandomParentIndex]);
	}
}

FGAIndividual UGACrossoverStrategy_PickRandomParentsSet::GenerateOffspring(const FGAIndividual& Parent1, const FGAIndividual& Parent2, const UGAOptimizerSubsystem* GASubsystem)
{
	// Gene union from the two parents, TSet avoids duplicated copies so we are avoiding duplicated cards
	TSet<int32> UnionCards;
	UnionCards.Append(Parent1.Chromosome);
	UnionCards.Append(Parent2.Chromosome);
	TArray<int32> UnionArray = UnionCards.Array();
	
	const int32 ChromosomeLenght = GASubsystem->GetGeneticAlgorithmConfig()->ChromosomeLenght;
    
	// Verifying if there is enough unique genes to generate the child (this shouldn't be happening)
	if (UnionArray.Num() < ChromosomeLenght)
	{
		UE_LOG(LogGeneticAlgorithm, Error, TEXT("GACrossoverStrategy_PickRandomParentsSet: Insufficient unique genes (%d). Need at least %d."), UnionArray.Num(), ChromosomeLenght);
		return Parent1; // Return first parent as a fallback
	}
    
	FGAIndividual Child;
	Child.Chromosome.Reserve(ChromosomeLenght);
	
	for (int32 i = 0; i < ChromosomeLenght; i++)
	{
		const int32 RandomIndex = GASubsystem->GetGARandomStream().RandRange(0, UnionArray.Num() - 1);
		Child.Chromosome.Add(UnionArray[RandomIndex]);
		//Explicar que hace removeatswap	
		UnionArray.RemoveAtSwap(RandomIndex, 1, false);
	}
    
	return Child;
}

void UGACrossoverStrategy_PickRandomParentsSet::CopyParentsToOffspring(const FGAIndividual& Parent1, const FGAIndividual& Parent2, TArray<FGAIndividual>& OutOffspring, FGACrossoverReportEntry& CrossoverReportEntry, int32 TargetOffspringSize)
{
	OutOffspring.Add(Parent1);
	CrossoverReportEntry.Offspring1Chromosome = Parent1.Chromosome;
    
	// Adding second parent only if there is space left in the offspring population
	if (OutOffspring.Num() < TargetOffspringSize)
	{
		OutOffspring.Add(Parent2);
		CrossoverReportEntry.Offspring2Chromosome = Parent2.Chromosome;
	}
}
