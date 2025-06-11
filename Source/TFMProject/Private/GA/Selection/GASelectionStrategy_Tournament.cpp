// © 2025 Beatriz Sabater Serna <dev@beatrizsabater.es>


#include "GA/Selection/GASelectionStrategy_Tournament.h"

#include "GA/GAConfigDataAsset.h"
#include "GA/GAOptimizerSubsystem.h"
#include "TFMProject/TFMProject.h"

void UGASelectionStrategy_Tournament::SelectParents(UObject* WorldContextObject, const TArray<FGAIndividual>& OldPopulation, TArray<FGAIndividual>& OutSelectedParents)
{
	if (OldPopulation.IsEmpty()) return;

	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!IsValid(World)) return;

	UGAOptimizerSubsystem* GASubsystem = World->GetSubsystem<UGAOptimizerSubsystem>();
	if (!IsValid(GASubsystem)) return;

	UGAConfigDataAsset* GAConfigDataAsset = GASubsystem->GetGeneticAlgorithmConfig();
	if (!IsValid(GAConfigDataAsset)) return;

	const int32 PopulationSize = OldPopulation.Num();
	const int32 NumToSelect = GAConfigDataAsset->PopulationSize;
	
	OutSelectedParents.Empty(NumToSelect);
	
	const int32 ActualTournamentSize = FMath::Max(2, FMath::Min(TournamentSize, PopulationSize));

	for (int32 i = 0; i < NumToSelect; ++i)
	{
		const FGAIndividual* Winner = nullptr;

		for (int32 j = 0; j < ActualTournamentSize; ++j)
		{
			const int32 RandomIndex = GASubsystem->GetGARandomStream().RandRange(0, PopulationSize - 1);
			const FGAIndividual& Contestant = OldPopulation[RandomIndex];

			if (Winner == nullptr || Contestant.Fitness > Winner->Fitness)
			{
				Winner = &Contestant;
			}
		}

		if (Winner)
		{
			OutSelectedParents.Add(*Winner);
		}
		else
		{
			UE_LOG(LogGeneticAlgorithm, Error, TEXT("UGASelectionStrategy_Tournament::SelectParents - Failed to select a winner in tournament %d."), i);
		}
	}
}
