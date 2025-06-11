// © 2025 Beatriz Sabater Serna <dev@beatrizsabater.es>


#include "UI/ClashAnalyzerWidget.h"

#include "Clash/ClashArrangementDataAsset.h"
#include "Clash/ClashSubsystem.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "GA/GAConfigDataAsset.h"
#include "GA/GAOptimizerSubsystem.h"
#include "GA/GeneticAlgorithmUtils.h"
#include "Units/UnitUtils.h"

void UClashAnalyzerWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// if (StartSimulationButton != nullptr)
	// {
	// 	StartSimulationButton->OnClicked.AddUniqueDynamic(this, &UClashAnalyzerWidget::HandleOnStartSimulationClicked);
	// }
	
	UGAOptimizerSubsystem* GAOptimizerSubsystem = GetWorld()->GetSubsystem<UGAOptimizerSubsystem>();
	if (IsValid(GAOptimizerSubsystem))
	{
		GAOptimizerSubsystem->OnOptimizationFinishedDelegate.AddUniqueDynamic(this, &UClashAnalyzerWidget::HandleOnOptimizationFinished);
	}
}

void UClashAnalyzerWidget::HandleOnStartSimulationClicked(UClashArrangementDataAsset* ClashArrangementDataAsset, UGAConfigDataAsset* GAConfigDataAsset)
{
	if (!IsValid(ClashArrangementDataAsset) || !IsValid(GAConfigDataAsset)) return;

	UClashSubsystem* ClashSubsystem = GetWorld()->GetSubsystem<UClashSubsystem>();
	UGAOptimizerSubsystem* GAOptimizerSubsystem = GetWorld()->GetSubsystem<UGAOptimizerSubsystem>();
	if (!IsValid(ClashSubsystem) || !IsValid(GAOptimizerSubsystem)) return;

	ClashSubsystem->InjectClashArrangement(ClashArrangementDataAsset);

	TArray<int32> AvailableCardsIDs;
	UUnitUtils::GetUniqueIDsFromCards(ClashArrangementDataAsset->ClashAvailableCards, AvailableCardsIDs); 
	GAOptimizerSubsystem->StartOptimization(GAConfigDataAsset, AvailableCardsIDs);
}

void UClashAnalyzerWidget::HandleOnOptimizationFinished(const FGAIndividual& OverallBestIndividual, bool bCompletedSuccessfully)
{
	if (IsValid(BestChromosomeTextBlock))
	{
		FString BestCromosomeString = UGeneticAlgorithmUtils::ChromosomeToString(OverallBestIndividual.Chromosome);
		BestChromosomeTextBlock->SetText(FText::FromString(BestCromosomeString));
	}

	if (IsValid(BestFitnessTextBlock))
	{
		BestFitnessTextBlock->SetText(FText::FromString(FString::SanitizeFloat(OverallBestIndividual.Fitness)));
	}
}
