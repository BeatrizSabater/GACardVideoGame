// © 2025 Beatriz Sabater Serna <dev@beatrizsabater.es>

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ClashAnalyzerWidget.generated.h"

struct FGAIndividual;
class UTextBlock;
class UGAConfigDataAsset;
class UClashArrangementDataAsset;
class UButton;
/**
 * 
 */
UCLASS()
class TFMPROJECT_API UClashAnalyzerWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	virtual void NativeOnInitialized() override;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	// UButton* StartSimulationButton = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* BestChromosomeTextBlock = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* BestFitnessTextBlock = nullptr;

	UFUNCTION(BlueprintCallable)
	void HandleOnStartSimulationClicked(UClashArrangementDataAsset* ClashArrangementDataAsset, UGAConfigDataAsset* GAConfigDataAsset);

	UFUNCTION()
	void HandleOnOptimizationFinished(const FGAIndividual& OverallBestIndividual, bool bCompletedSuccessfully);
};
