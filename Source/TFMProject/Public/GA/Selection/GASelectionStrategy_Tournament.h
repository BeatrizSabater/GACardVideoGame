// © 2025 Beatriz Sabater Serna <dev@beatrizsabater.es>

#pragma once

#include "CoreMinimal.h"
#include "GA/GAOperators.h"
#include "GASelectionStrategy_Tournament.generated.h"

/**
 * 
 */
UCLASS()
class TFMPROJECT_API UGASelectionStrategy_Tournament : public UGASelectionStrategyAbstract
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Strategy Settings", meta=(ClampMin=2))
	int32 TournamentSize = 3;
	
	virtual void SelectParents(UObject* WorldContextObject, const TArray<FGAIndividual>& OldPopulation, TArray<FGAIndividual>& OutSelectedParents) override;
};
