// © 2025 Beatriz Sabater Serna <dev@beatrizsabater.es>

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GeneticAlgorithmUtils.generated.h"

class UCardDataAsset;


UCLASS()
class TFMPROJECT_API UGeneticAlgorithmUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintPure)
	static FString ChromosomeToString(const TArray<int32>& Chromosome);
	
};
