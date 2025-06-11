// © 2025 Beatriz Sabater Serna <dev@beatrizsabater.es>


#include "GA/GeneticAlgorithmUtils.h"


FString UGeneticAlgorithmUtils::ChromosomeToString(const TArray<int32>& Chromosome)
{
	FString ChromosomeString = "";
	for (const int32 CardID : Chromosome)
	{
		ChromosomeString += FString::Printf(TEXT("%d "), CardID);
	}
	
	return ChromosomeString;
}

