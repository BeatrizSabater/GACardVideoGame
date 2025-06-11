// © 2025 Beatriz Sabater Serna <dev@beatrizsabater.es>

#pragma once

#include "CoreMinimal.h"
#include "GA/GAOperators.h"
#include "GACrossoverStrategy_PickRandomParentsSet.generated.h"

class UGAOptimizerSubsystem;


UCLASS()
class TFMPROJECT_API UGACrossoverStrategy_PickRandomParentsSet : public UGACrossoverStrategyAbstract
{
	GENERATED_BODY()

public:

	virtual void PerformCrossover(const UObject* WorldContextObject, const TArray<FGAIndividual>& Parents, TArray<FGAIndividual>& OutOffspring, TArray<FGACrossoverReportEntry>& OutReportEntries) override;

private:

	FGAIndividual GenerateOffspring(const FGAIndividual& Parent1, const FGAIndividual& Parent2, const UGAOptimizerSubsystem* GASubsystem);
	
	void CopyParentsToOffspring(const FGAIndividual& Parent1, const FGAIndividual& Parent2, TArray<FGAIndividual>& OutOffspring, FGACrossoverReportEntry& CrossoverReportEntry,	int32 TargetOffspringSize);
};
