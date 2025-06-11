// © 2025 Beatriz Sabater Serna <dev@beatrizsabater.es>


#include "GA/Fitness/GAFitnessStrategy_FixedSolution.h"

float UGAFitnessStrategy_FixedSolution::CalculateFitness(UObject* WorldContextObject, const FGAIndividual& Individual, const TArray<int32>& CardPool)
{
	TArray FixedChromosome = { 7, 11, 20, 23, 10};

	int32 IdenticalCards = 0;

	for (auto Chromosome : Individual.Chromosome)
	{
		if (FixedChromosome.Contains(Chromosome))
		{
			IdenticalCards++;
		}
	}
	
	return static_cast<float>(IdenticalCards)/Individual.Chromosome.Num();
}
